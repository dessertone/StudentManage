#include <bits/stdc++.h>
#include "bin_search.h"
#ifndef __STUDENT_INFOR_MANAGE_H__
#define __STUDENT_INFOR_MANAGE_H__
using namespace std;
static const long INCREMENT_OF_INDEX_TABLE = 100;
struct StuInforType
{
    char name[5];     // ����
    char number[14]; // ѧ��
    char sex[4];        // �Ա�
    bool isexpelled; // �Ƿ񱻿���
    char dep[5];     // ϵ
    int age;         // ����
};

struct IndexItemType
{
    char number[14];
    long offset;
};

bool operator<=(const char number[14], const IndexItemType &item);
bool operator<(const char number[14], const IndexItemType &item);
bool operator==(const char number[14], const IndexItemType &item);
bool operator<(const IndexItemType &first, const IndexItemType &second);

class studentinformanage
{
private:
    IndexItemType *pIndexTable;
    FILE *pFStuData;
    long size;
    long maxsize;
    /* �������� */
    void Display(const StuInforType sstuinfor); // ��ʾ��¼
    void DisplayAll();                          // ��ʾ���м�¼
    void Search();                              // ���Ҽ�¼
    void Delete();                              // ɾ����¼
    void Input();                               // ��Ӽ�¼
    void Update();                              // ���¼�¼
public:
    studentinformanage();
    virtual ~studentinformanage();
    void run();
};

studentinformanage::studentinformanage() // ���캯��
{
    if ((pFStuData = fopen("student.dat", "rb+")) == NULL)
        cout << "�ļ�������" << endl;
    if ((pFStuData = fopen("student.dat", "wb+")) == NULL)
    {
        cout << "�ļ�����ʧ��" << endl;
        exit(1);
    }

    FILE *pFStuIndex;
    if ((pFStuIndex = fopen("sutdent.idx", "rb+")) == NULL)
    {
        size = 0;
        maxsize = INCREMENT_OF_INDEX_TABLE;
        pIndexTable = new IndexItemType[maxsize];
    }
    else
    {
        fseek(pFStuIndex, 0, SEEK_END);
        size = ftell(pFStuIndex) / sizeof(IndexItemType);
        maxsize = size + INCREMENT_OF_INDEX_TABLE;
        pIndexTable = new IndexItemType[maxsize];
        fseek(pFStuIndex, 0, SEEK_SET);
        for (int pos = 0; pos < size; ++pos)
        {
            fread(&pIndexTable[pos], sizeof(IndexItemType), 1, pFStuIndex);
        }
        fclose(pFStuIndex);
    }
}

studentinformanage::~studentinformanage() // ��������
{
    fclose(pFStuData);
    FILE *pFStuIndex;
    if ((pFStuIndex = fopen("student.idx", "wb+")) == NULL)
    {
        cout << "�������ļ�ʧ��" << endl;
        exit(1);
    }
    for (int pos = 0; pos < size; pos++)
    {
        fwrite(&pIndexTable[pos], sizeof(IndexItemType), 1, pFStuIndex);
    }
    fclose(pFStuIndex);
    delete[] pIndexTable;
}

void studentinformanage::Search()
{
    char num[14];
    cout << "����ѧ��:";
    cin >> num;
    while (cin.get() != '\n')
        ;
    int pos = BinSerach(pIndexTable, size, num);
    if (pos == -1)
        cout << "���޴���" << endl;
    else
    {
        StuInforType stuInfor;
        fseek(pFStuData, pIndexTable[pos].offset, SEEK_SET);
        fread(&stuInfor, sizeof(StuInforType), 1, pFStuData);
        Display(stuInfor);
    }
}

void studentinformanage::Delete()
{
    char num[14];
    cout << "����ѧ��:";
    cin >> num;
    while (cin.get() != '\n')
        ;
    int pos = BinSerach(pIndexTable, size, num);
    if (pos == -1)
        cout << "���˲����ڻ��ѱ�ɾ��" << endl;
    else
    {
        StuInforType stuinfor;
        fseek(pFStuData, pIndexTable[pos].offset, SEEK_SET);   // ��λ�ļ�λ��
        fread(&stuinfor, sizeof(StuInforType), 1, pFStuData);  // ��ȡ��Ϣ
        stuinfor.isexpelled = true;                            // �޸���Ϣ
        fseek(pFStuData, pIndexTable[pos].offset, SEEK_SET);   // ��λ�ļ�λ��
        fwrite(&stuinfor, sizeof(StuInforType), 1, pFStuData); // ����Ϣ����¼���ļ�
        for (int i = pos + 1; i < size; ++i)
        {
            pIndexTable[i - 1] = pIndexTable[i];
        }
        size--;
        Display(stuinfor);
        cout << "�����ɹ�!" << endl;
    }
}

void studentinformanage::Input()
{
    char num[14];
    cout << "�������ѧ����ѧ��:";
    cin >> num;
    while (cin.get() != '\n')
        ;
    if (BinSerach(pIndexTable, size, num) > 0)
    {
        cout << "�Ѵ��ڸ�ѧ��!" << endl;
        return;
    }
    else
    {
        StuInforType stuinfor;
        strcpy(stuinfor.number, num);
        cout << "��������:"; cin >> stuinfor.name;
        cout << "�����Ա�:"; cin >> stuinfor.sex;
        cout << "����ϵ:";  cin >> stuinfor.dep;
        cout << "��������:"; cin >> stuinfor.age;
        stuinfor.isexpelled = false;
        while (cin.get() != '\n');
        fseek(pFStuData, 0, SEEK_END);
        long offset = ftell(pFStuData); // ��¼��Ϣ��ƫ��λ��
        fwrite(&stuinfor, sizeof(StuInforType), 1, pFStuData);
        // �����������������
        if (size >= maxsize)
        {
            maxsize += INCREMENT_OF_INDEX_TABLE; // �����������
            IndexItemType *ptemp;
            ptemp = new IndexItemType[maxsize];
            for (int pos = 0; pos < size; ++pos)
                ptemp[pos] = pIndexTable[pos];
            delete[] pIndexTable;
            pIndexTable = ptemp;
        }
        int j;
        for (j = size - 1;  j >=0 && stuinfor.number < pIndexTable[j]; j--)
        {
            pIndexTable[j + 1] = pIndexTable[j];
        }
        strcpy(pIndexTable[j + 1].number, stuinfor.number);
        pIndexTable[j + 1].offset = offset;
        ++size;
        cout << size;
    }
}

void studentinformanage::Update()
{
    StuInforType stuinfor;
    cout << "����ѧ��:";
    char num[14];
    cin >> num;
    while(cin.get()!= '\n');
    int pos = BinSerach(pIndexTable, size, num);
    if (pos == -1)
    {
        cout << "���޴���" << endl;
        return;
    }
    else
    {
        int select;
        do{
            cout << "��ѡ�����:\n1.ѧ��\n 2.����\n 3.�Ա�\n 4.ϵ\n 5.����\n 6.�˳�" << endl;
            cin >> select;
           switch (select)
            {
            case 1:
                cout << "����ѧ��:"; cin >> stuinfor.number;
                if(BinSerach(pIndexTable, size, stuinfor.number) >= 0)
                {
                    cout << "���д�ѧ��" << endl;
                    return;
                }
                break;
            case 2:
                cout << "��������:"; cin >> stuinfor.name;
                break;
           case 3:
                cout << "�����Ա�:"; cin >> stuinfor.sex;
                break;
           case 4:
                cout << "����ϵ:"; cin >> stuinfor.dep;
                break;
            case 5:
                cout << "��������:"; cin >> stuinfor.age;
                break;
           }
    }while(select!=6);
    fseek(pFStuData,pIndexTable[pos].offset, SEEK_SET);
    fwrite(&stuinfor, sizeof(StuInforType), 1, pFStuData);
    
    }
}

void studentinformanage::run()
{
    int select;
    do{
        cout << "��ѡ��:\n1.��ʾ���м�¼\n 2.���Ҽ�¼\n 3.ɾ����¼\n 4.��Ӽ�¼\n 5.���¼�¼\n 6.�˳�" << endl;
        cin >> select;
        switch (select)
        {
        case 1:
            DisplayAll();
            break;
        case 2:
            Search();
            break;
        case 3:
            Delete();
            break;
        case 4:
            Input();
            break;
        case 5:
            Update();
            break;
        }
    }while(select!=6);
}

void studentinformanage::Display(const StuInforType sstuinfor)
{
    cout << "number:    " << sstuinfor.number << endl;
    cout << "name:    " << sstuinfor.name << endl;
    cout << "sex:    " << sstuinfor.sex << endl;
    cout << "dep:    " << sstuinfor.dep << endl;
    cout << "age:    " << sstuinfor.age << endl;
}

void studentinformanage::DisplayAll()
{
    cout << setw(14) << "number" << setw(10) << "name" << setw(8) << "sex" << setw(8) << "dep" << setw(10) << "age" << endl;
    for (int pos = 0; pos < size; pos++)
    {
        StuInforType stuinfor;
        fseek(pFStuData, pIndexTable[pos].offset, SEEK_SET);
        fread(&stuinfor, sizeof(StuInforType), 1, pFStuData);
        cout << setw(14) << stuinfor.number << setw(10) << stuinfor.name << setw(8) << stuinfor.sex << setw(8) << stuinfor.dep << setw(10) << stuinfor.age << endl;
    }
}

bool operator<=(const char number[14], const IndexItemType &item) {return strcmp(number, item.number) <= 0;}

bool operator<(const char number[14], const IndexItemType &item) {return strcmp(number, item.number) < 0;}

bool operator==(const char number[14], const IndexItemType &item) {return strcmp(number, item.number) == 0;}

bool operator<(const IndexItemType &first, const IndexItemType &second) {return strcmp(first.number, second.number) < 0;}
#endif