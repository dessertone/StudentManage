#include <bits/stdc++.h>
#include "bin_search.h"
#ifndef __STUDENT_INFOR_MANAGE_H__
#define __STUDENT_INFOR_MANAGE_H__
using namespace std;
static const long INCREMENT_OF_INDEX_TABLE = 100;
struct StuInforType
{
    char name[5];     // 姓名
    char number[14]; // 学号
    char sex[4];        // 性别
    bool isexpelled; // 是否被开除
    char dep[5];     // 系
    int age;         // 年龄
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
    /* 辅助函数 */
    void Display(const StuInforType sstuinfor); // 显示记录
    void DisplayAll();                          // 显示所有记录
    void Search();                              // 查找记录
    void Delete();                              // 删除记录
    void Input();                               // 添加记录
    void Update();                              // 更新记录
public:
    studentinformanage();
    virtual ~studentinformanage();
    void run();
};

studentinformanage::studentinformanage() // 构造函数
{
    if ((pFStuData = fopen("student.dat", "rb+")) == NULL)
        cout << "文件不存在" << endl;
    if ((pFStuData = fopen("student.dat", "wb+")) == NULL)
    {
        cout << "文件创建失败" << endl;
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

studentinformanage::~studentinformanage() // 析构函数
{
    fclose(pFStuData);
    FILE *pFStuIndex;
    if ((pFStuIndex = fopen("student.idx", "wb+")) == NULL)
    {
        cout << "打开索引文件失败" << endl;
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
    cout << "输入学号:";
    cin >> num;
    while (cin.get() != '\n')
        ;
    int pos = BinSerach(pIndexTable, size, num);
    if (pos == -1)
        cout << "查无此人" << endl;
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
    cout << "输入学号:";
    cin >> num;
    while (cin.get() != '\n')
        ;
    int pos = BinSerach(pIndexTable, size, num);
    if (pos == -1)
        cout << "此人不存在或已被删除" << endl;
    else
    {
        StuInforType stuinfor;
        fseek(pFStuData, pIndexTable[pos].offset, SEEK_SET);   // 定位文件位置
        fread(&stuinfor, sizeof(StuInforType), 1, pFStuData);  // 读取信息
        stuinfor.isexpelled = true;                            // 修改信息
        fseek(pFStuData, pIndexTable[pos].offset, SEEK_SET);   // 定位文件位置
        fwrite(&stuinfor, sizeof(StuInforType), 1, pFStuData); // 将信息重新录入文件
        for (int i = pos + 1; i < size; ++i)
        {
            pIndexTable[i - 1] = pIndexTable[i];
        }
        size--;
        Display(stuinfor);
        cout << "开除成功!" << endl;
    }
}

void studentinformanage::Input()
{
    char num[14];
    cout << "输入添加学生的学号:";
    cin >> num;
    while (cin.get() != '\n')
        ;
    if (BinSerach(pIndexTable, size, num) > 0)
    {
        cout << "已存在该学号!" << endl;
        return;
    }
    else
    {
        StuInforType stuinfor;
        strcpy(stuinfor.number, num);
        cout << "输入姓名:"; cin >> stuinfor.name;
        cout << "输入性别:"; cin >> stuinfor.sex;
        cout << "输入系:";  cin >> stuinfor.dep;
        cout << "输入年龄:"; cin >> stuinfor.age;
        stuinfor.isexpelled = false;
        while (cin.get() != '\n');
        fseek(pFStuData, 0, SEEK_END);
        long offset = ftell(pFStuData); // 记录信息的偏移位置
        fwrite(&stuinfor, sizeof(StuInforType), 1, pFStuData);
        // 如果索引项到达最大容量
        if (size >= maxsize)
        {
            maxsize += INCREMENT_OF_INDEX_TABLE; // 增加最大容量
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
    cout << "输入学号:";
    char num[14];
    cin >> num;
    while(cin.get()!= '\n');
    int pos = BinSerach(pIndexTable, size, num);
    if (pos == -1)
    {
        cout << "查无此人" << endl;
        return;
    }
    else
    {
        int select;
        do{
            cout << "请选择更改:\n1.学号\n 2.姓名\n 3.性别\n 4.系\n 5.年龄\n 6.退出" << endl;
            cin >> select;
           switch (select)
            {
            case 1:
                cout << "输入学号:"; cin >> stuinfor.number;
                if(BinSerach(pIndexTable, size, stuinfor.number) >= 0)
                {
                    cout << "已有此学号" << endl;
                    return;
                }
                break;
            case 2:
                cout << "输入姓名:"; cin >> stuinfor.name;
                break;
           case 3:
                cout << "输入性别:"; cin >> stuinfor.sex;
                break;
           case 4:
                cout << "输入系:"; cin >> stuinfor.dep;
                break;
            case 5:
                cout << "输入年龄:"; cin >> stuinfor.age;
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
        cout << "请选择:\n1.显示所有记录\n 2.查找记录\n 3.删除记录\n 4.添加记录\n 5.更新记录\n 6.退出" << endl;
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