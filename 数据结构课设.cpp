#include<iostream>
#include<cstdio>
#include<fstream>
#include<algorithm>
#include<map>
#include<vector>
using namespace std;

const int MAX = 256;
#define LL long long 

struct HTNode {
    unsigned char data;                  //代表字符0
    int weight;                 //权值
    int parent, lchild, rchild; //双亲和孩子
    HTNode() {
        weight = parent = lchild = rchild = -1; //初始化为-1
    }
};

LL inputlen;   //代表input.txt字符的长度(个数)
int n = 0; //代表输入字符的种类
HTNode* arr;//存放哈夫曼树
map<char, vector<int>> mp;//存放字符和字符对应的哈夫曼编码
unsigned char* char_huffman;//存放输入的元素

LL openfile(char* name);//读取文件，并返回文件长度
void Count(LL filelen);//统计字符频率
void CreateHuffmanTree();//构造哈夫曼树
void HaffmanCode();//生成哈夫曼编码
void outPutTree();//输出编码和哈夫曼树
void Binarycode();//将input.txt压缩，将二进制代码写入到output.txt中
int Translate();//对output.txt文件进行译码，并写入original
unsigned char int_to_char(int* a);//将8位01序列转换为ASCII码
void char_to_int(int x, int* a);//将数字转换为二进制

int main() {
    //freopen("in.txt","r",stdin);
    cout << "请输入要压缩的文件名字" << endl;//input.txt
    char filename[MAX];
    cin.getline(filename, MAX);//输入文件名字
    //char filename[MAX] = "input1.txt";
    //char filename[MAX] = "input2.txt";
    //char filename[MAX] = "input3.txt";
    arr = new HTNode[2 * MAX - 1];//为存储二叉树的数组提前分配空间

    inputlen = openfile(filename);//读取文件并返回文件长度
    Count(inputlen);//统计字符频率
    CreateHuffmanTree();//创建哈夫曼树
    HaffmanCode();//生成哈夫曼编码
    outPutTree();//输出字符对应编码和哈夫曼树
    Binarycode();//将输入的txt文件压缩到output.txt中
    int outputlen = Translate();//解压output.txt，将解压内容写进original.txt中
    return 0;
}

LL openfile(char* name) {
    //读取文件的字符
    FILE* file1 = NULL;
    file1 = fopen(name, "rb");//以只读二进制的方式打开文件
    if (file1 == NULL) {
        cout << "打开文件失败！" << endl;
        exit(1);
    }
    cout << "input.txt文件打开成功！" << endl;
    cout << endl;
    //获得文件长度
    fseek(file1, 0, SEEK_END);//文件指针移动到末尾
    LL filelen = ftell(file1);//获得文件当前指针位置，即为文件长度
    rewind(file1);//将文件指针移动到开头，准备读取
    //char* char_huffman = new char[filelen];
    char_huffman = new unsigned char[filelen];
    for (LL i = 0; i < filelen; i++) {//将缓冲区数据设置0
        char_huffman[i] = 0;
    }
    fread(char_huffman, 1, filelen, file1);//读取文件
    fclose(file1);
    return filelen;
}

void Count(LL filelen) {
    for (LL i = 0; i < filelen; i++) {
        int flag = 0;
        for (int j = 0; j < n; j++) {
            if (arr[j].data == char_huffman[i]) {
                flag = 1;
                arr[j].weight++;
                break;
            }
        }
        if (!flag) {
            arr[n].data = char_huffman[i];
            arr[n].weight = 1;
            n++;
        }
    }
}

void CreateHuffmanTree() { //生成哈夫曼树
    for (int i = n; i < 2 * n - 1; i++) {
        //min1和min2尽量选的大一点，如果数据量过大min1和min2过少，构建树的时候会出问题
        LL min1 = 10000000; LL min2 = min1;//min1第一小，min2第二小 
        int x1 = -1, x2 = -1;//分别记录两者下标
        for (int j = 0; j < i; j++) {
            if (arr[j].parent == -1) {
                if (arr[j].weight < min1) {
                    min2 = min1;
                    x2 = x1;
                    min1 = arr[j].weight;
                    x1 = j;
                }
                else if (arr[j].weight < min2) {
                    min2 = arr[j].weight;
                    x2 = j;
                }
            }
        }
        arr[x1].parent = arr[x2].parent = i;
        arr[i].weight = min1 + min2;
        arr[i].lchild = x1;
        arr[i].rchild = x2;
    }
}

//哈夫曼编码
void HaffmanCode()
{
    for (int i = 0; i < n; i++) {
        int t = i; //代表儿子
        vector<int> v;
        int parent = arr[i].parent;
        for (int j = parent; j != -1; t = j, j = arr[j].parent) {
            if (arr[j].lchild == t) {
                //v.insert(v.begin(),0);在编码首部插入0
                v.push_back(0);//在编码末尾插入0
            }
            else {
                v.push_back(1);
            }
        }
        reverse(v.begin(), v.end());//翻转容器
        mp[arr[i].data] = v;
    }
}

void outPutTree() {//输出编码和哈夫曼树
    //输出字符对应的编码
    cout << "字符对应的编码为:" << endl;
    for (map<char, vector<int>>::iterator it = mp.begin(); it != mp.end(); it++) {
        printf("%c ", it->first);
        //cout<<it->first<<" ";
        for (int i = 0; i < (*it).second.size(); i++) {
            printf("%d", (*it).second[i]);
            //cout<<(*it).second[i];
        }
        printf("\n");
    }
    /*
    for (auto p : mp)
    {
        cout << p.first << " " << p.second << endl;
    }
    */
    //输出哈夫曼树
    printf("输出哈夫曼树:\n");
    printf("下标\t权值\t父结点\t左孩子\t右孩子\t结点\n");
    for (int i = 0; i < 2 * n - 1; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%c\t\n", i, arr[i].weight, arr[i].parent, arr[i].lchild, arr[i].rchild, arr[i].data);
    }
    printf("\n");
}

void Binarycode() {
    ofstream file1("output.txt");//创建一个文件流向output.txt
    if (!file1.is_open()) {
        printf("output.txt文件打开失败\n");
        //cout<<"output.txt文件打开失败"<<endl;
        exit(1);
    }
    printf("压缩编码时，output.txt文件打开成功！\n");
    //cout<<"压缩编码时，output.txt文件打开成功！"<<endl;
    printf("\n");
    int buffer[8] = { 0 };//用作缓冲区
    int count = 0;//缓冲字节，初始为0
    //输出编码到output.txt文件中
    //每次读入8个字符
    for (int i = 0; i < inputlen; i++) {
        int x;
        x = char_huffman[i];
        for (int j = 0; j < mp[x].size(); j++) {
            if (count == 8) {
                if (count == 8) {
                file1 << int_to_char(buffer);//将8位01序列转换为ASCII码
                count = 0;
            }
            buffer[count] = mp[x][j];//读进去的是0或者1，而不能是'0',因为它的assci码是48,这样直接就读入的是48了
            count++;
        }
    }
    //最后一位写进去
    if (count != 0) {
        file1 << int_to_char(buffer);//将8位01序列转换为ASCII码
    }
    char lackcount = count;//缺位数，以char的形式放再最后
    file1 << lackcount;
    file1.close();
}

int Translate() {
    FILE* file1 = NULL;
    file1 = fopen("output.txt", "rb");
    if (file1 == NULL) {
        cout << "写入文件时,文件output.txt打开失败！" << endl;
        exit(1);
    }
    cout << "译码时，output.txt文件打开成功！" << endl;
    cout << endl;
    //获得文件长度
    fseek(file1, 0, SEEK_END);//文件指针移到末尾
    LL filelen = ftell(file1);//获得文当前指针位置，即为文件长度
    //读取文件
    rewind(file1);//将文件指针移动到开头准备读取
    unsigned char* buf = new unsigned char[filelen];
    for (int i = 0; i < filelen; i++) {//将缓冲区的数据设置为0
        buf[i] = 0;
    }
    fread(buf, 1, filelen, file1);//读取文件
    fclose(file1);//关闭文件
    //输出不出来
    /*
    for(int i=0;i<filelen;i++){
        cout<<buf[i]<<" ";
    }
    */
    vector<int> source;//存放读出的编码
    int a[8];//存储数字
    for (int i = 0; i < 8; i++) {
        a[i] = 0;
    }
    for (int i = 0; i < filelen - 2; i++) {
        //if(buf[i] == 13 && buf[i+1] == 10){//碰见回车换行符就略过回车换行符/r/t
          //  continue;
        //}
        char_to_int(buf[i], a);
        for (int j = 0; j < 8; j++) {
            source.push_back(a[j]);
        }
    }
    //倒数第二位是一个字符,最后一位是缺位数，缺位数不读
    int lackcount = buf[filelen - 1];
    char_to_int(buf[filelen - 2], a);
    for (int j = 0; j < lackcount; j++) {
        source.push_back(a[j]);
    }

    //输入到文件original.txt中
    ofstream file2("original.txt", ios::binary);
    //ofstream file2("original.txt");
    if (!file2.is_open()) {
        printf("文件original.txt打开失败！");
        //cout<<"文件original.txt打开失败！"<<endl;
        exit(1);
    }
    printf("译码时，original.txt文件打开成功！");
    //cout<<"译码时，original.txt文件打开成功！"<<endl;
    printf("\n");
    int q = 2 * n - 2;
    for (int i = 0; i < source.size(); i++) {
        if (source[i] == 0) {
            q = arr[q].lchild;
        }
        else if (source[i] == 1) {
            q = arr[q].rchild;
        }
        if (arr[q].lchild == -1 && arr[q].rchild == -1) {
            if (arr[q].data != 13)//  /r和/t去掉其中一个,windows中/r和/t均可以换行,不去掉就出现换行两次的情况
                file2 << arr[q].data;
            q = 2 * n - 2;
        }
    }
    return filelen;
}
//将8位01序列转换为ASCII码
unsigned char int_to_char(int* a) {
    unsigned char x;
    x = a[0] * 128 + a[1] * 64 + a[2] * 32 + a[3] * 16 + a[4] * 8 + a[5] * 4 + a[6] * 2 + a[7] * 1;
    return x;
}
//将数字转换为二进制
void char_to_int(int x, int* a) {
    for (int i = 7; i >= 0; i--) {
        a[i] = x % 2;
        x = x / 2;
    }
}
