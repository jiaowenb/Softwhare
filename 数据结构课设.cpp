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
    unsigned char data;                  //�����ַ�0
    int weight;                 //Ȩֵ
    int parent, lchild, rchild; //˫�׺ͺ���
    HTNode() {
        weight = parent = lchild = rchild = -1; //��ʼ��Ϊ-1
    }
};

LL inputlen;   //����input.txt�ַ��ĳ���(����)
int n = 0; //���������ַ�������
HTNode* arr;//��Ź�������
map<char, vector<int>> mp;//����ַ����ַ���Ӧ�Ĺ���������
unsigned char* char_huffman;//��������Ԫ��

LL openfile(char* name);//��ȡ�ļ����������ļ�����
void Count(LL filelen);//ͳ���ַ�Ƶ��
void CreateHuffmanTree();//�����������
void HaffmanCode();//���ɹ���������
void outPutTree();//�������͹�������
void Binarycode();//��input.txtѹ�����������ƴ���д�뵽output.txt��
int Translate();//��output.txt�ļ��������룬��д��original
unsigned char int_to_char(int* a);//��8λ01����ת��ΪASCII��
void char_to_int(int x, int* a);//������ת��Ϊ������

int main() {
    //freopen("in.txt","r",stdin);
    cout << "������Ҫѹ�����ļ�����" << endl;//input.txt
    char filename[MAX];
    cin.getline(filename, MAX);//�����ļ�����
    //char filename[MAX] = "input1.txt";
    //char filename[MAX] = "input2.txt";
    //char filename[MAX] = "input3.txt";
    arr = new HTNode[2 * MAX - 1];//Ϊ�洢��������������ǰ����ռ�

    inputlen = openfile(filename);//��ȡ�ļ��������ļ�����
    Count(inputlen);//ͳ���ַ�Ƶ��
    CreateHuffmanTree();//������������
    HaffmanCode();//���ɹ���������
    outPutTree();//����ַ���Ӧ����͹�������
    Binarycode();//�������txt�ļ�ѹ����output.txt��
    int outputlen = Translate();//��ѹoutput.txt������ѹ����д��original.txt��
    return 0;
}

LL openfile(char* name) {
    //��ȡ�ļ����ַ�
    FILE* file1 = NULL;
    file1 = fopen(name, "rb");//��ֻ�������Ƶķ�ʽ���ļ�
    if (file1 == NULL) {
        cout << "���ļ�ʧ�ܣ�" << endl;
        exit(1);
    }
    cout << "input.txt�ļ��򿪳ɹ���" << endl;
    cout << endl;
    //����ļ�����
    fseek(file1, 0, SEEK_END);//�ļ�ָ���ƶ���ĩβ
    LL filelen = ftell(file1);//����ļ���ǰָ��λ�ã���Ϊ�ļ�����
    rewind(file1);//���ļ�ָ���ƶ�����ͷ��׼����ȡ
    //char* char_huffman = new char[filelen];
    char_huffman = new unsigned char[filelen];
    for (LL i = 0; i < filelen; i++) {//����������������0
        char_huffman[i] = 0;
    }
    fread(char_huffman, 1, filelen, file1);//��ȡ�ļ�
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

void CreateHuffmanTree() { //���ɹ�������
    for (int i = n; i < 2 * n - 1; i++) {
        //min1��min2����ѡ�Ĵ�һ�㣬�������������min1��min2���٣���������ʱ��������
        LL min1 = 10000000; LL min2 = min1;//min1��һС��min2�ڶ�С 
        int x1 = -1, x2 = -1;//�ֱ��¼�����±�
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

//����������
void HaffmanCode()
{
    for (int i = 0; i < n; i++) {
        int t = i; //�������
        vector<int> v;
        int parent = arr[i].parent;
        for (int j = parent; j != -1; t = j, j = arr[j].parent) {
            if (arr[j].lchild == t) {
                //v.insert(v.begin(),0);�ڱ����ײ�����0
                v.push_back(0);//�ڱ���ĩβ����0
            }
            else {
                v.push_back(1);
            }
        }
        reverse(v.begin(), v.end());//��ת����
        mp[arr[i].data] = v;
    }
}

void outPutTree() {//�������͹�������
    //����ַ���Ӧ�ı���
    cout << "�ַ���Ӧ�ı���Ϊ:" << endl;
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
    //�����������
    printf("�����������:\n");
    printf("�±�\tȨֵ\t�����\t����\t�Һ���\t���\n");
    for (int i = 0; i < 2 * n - 1; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%c\t\n", i, arr[i].weight, arr[i].parent, arr[i].lchild, arr[i].rchild, arr[i].data);
    }
    printf("\n");
}

void Binarycode() {
    ofstream file1("output.txt");//����һ���ļ�����output.txt
    if (!file1.is_open()) {
        printf("output.txt�ļ���ʧ��\n");
        //cout<<"output.txt�ļ���ʧ��"<<endl;
        exit(1);
    }
    printf("ѹ������ʱ��output.txt�ļ��򿪳ɹ���\n");
    //cout<<"ѹ������ʱ��output.txt�ļ��򿪳ɹ���"<<endl;
    printf("\n");
    int buffer[8] = { 0 };//����������
    int count = 0;//�����ֽڣ���ʼΪ0
    //������뵽output.txt�ļ���
    //ÿ�ζ���8���ַ�
    for (int i = 0; i < inputlen; i++) {
        int x;
        x = char_huffman[i];
        for (int j = 0; j < mp[x].size(); j++) {
            if (count == 8) {
                if (count == 8) {
                file1 << int_to_char(buffer);//��8λ01����ת��ΪASCII��
                count = 0;
            }
            buffer[count] = mp[x][j];//����ȥ����0����1����������'0',��Ϊ����assci����48,����ֱ�ӾͶ������48��
            count++;
        }
    }
    //���һλд��ȥ
    if (count != 0) {
        file1 << int_to_char(buffer);//��8λ01����ת��ΪASCII��
    }
    char lackcount = count;//ȱλ������char����ʽ�������
    file1 << lackcount;
    file1.close();
}

int Translate() {
    FILE* file1 = NULL;
    file1 = fopen("output.txt", "rb");
    if (file1 == NULL) {
        cout << "д���ļ�ʱ,�ļ�output.txt��ʧ�ܣ�" << endl;
        exit(1);
    }
    cout << "����ʱ��output.txt�ļ��򿪳ɹ���" << endl;
    cout << endl;
    //����ļ�����
    fseek(file1, 0, SEEK_END);//�ļ�ָ���Ƶ�ĩβ
    LL filelen = ftell(file1);//����ĵ�ǰָ��λ�ã���Ϊ�ļ�����
    //��ȡ�ļ�
    rewind(file1);//���ļ�ָ���ƶ�����ͷ׼����ȡ
    unsigned char* buf = new unsigned char[filelen];
    for (int i = 0; i < filelen; i++) {//������������������Ϊ0
        buf[i] = 0;
    }
    fread(buf, 1, filelen, file1);//��ȡ�ļ�
    fclose(file1);//�ر��ļ�
    //���������
    /*
    for(int i=0;i<filelen;i++){
        cout<<buf[i]<<" ";
    }
    */
    vector<int> source;//��Ŷ����ı���
    int a[8];//�洢����
    for (int i = 0; i < 8; i++) {
        a[i] = 0;
    }
    for (int i = 0; i < filelen - 2; i++) {
        //if(buf[i] == 13 && buf[i+1] == 10){//�����س����з����Թ��س����з�/r/t
          //  continue;
        //}
        char_to_int(buf[i], a);
        for (int j = 0; j < 8; j++) {
            source.push_back(a[j]);
        }
    }
    //�����ڶ�λ��һ���ַ�,���һλ��ȱλ����ȱλ������
    int lackcount = buf[filelen - 1];
    char_to_int(buf[filelen - 2], a);
    for (int j = 0; j < lackcount; j++) {
        source.push_back(a[j]);
    }

    //���뵽�ļ�original.txt��
    ofstream file2("original.txt", ios::binary);
    //ofstream file2("original.txt");
    if (!file2.is_open()) {
        printf("�ļ�original.txt��ʧ�ܣ�");
        //cout<<"�ļ�original.txt��ʧ�ܣ�"<<endl;
        exit(1);
    }
    printf("����ʱ��original.txt�ļ��򿪳ɹ���");
    //cout<<"����ʱ��original.txt�ļ��򿪳ɹ���"<<endl;
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
            if (arr[q].data != 13)//  /r��/tȥ������һ��,windows��/r��/t�����Ի���,��ȥ���ͳ��ֻ������ε����
                file2 << arr[q].data;
            q = 2 * n - 2;
        }
    }
    return filelen;
}
//��8λ01����ת��ΪASCII��
unsigned char int_to_char(int* a) {
    unsigned char x;
    x = a[0] * 128 + a[1] * 64 + a[2] * 32 + a[3] * 16 + a[4] * 8 + a[5] * 4 + a[6] * 2 + a[7] * 1;
    return x;
}
//������ת��Ϊ������
void char_to_int(int x, int* a) {
    for (int i = 7; i >= 0; i--) {
        a[i] = x % 2;
        x = x / 2;
    }
}
