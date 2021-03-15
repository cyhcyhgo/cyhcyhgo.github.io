/*#include <iostream>
using namespace std;
struct STRING
{
    char *str;//字符指针指向字符串
    int ct;//字符串的长度
};
//定义函数 set   show
void set(STRING &str, char *pstr)
{
    int i=0;
    for(;pstr[i];i++);
    str.ct=i;
    str.str=new char[i];
    str.str=pstr;
}
void show(STRING str, int ct=9999)
{
    int i;
    for(i=0;str.str[i]&&i<ct;i++) cout<<str.str[i];
    cout<<endl;
}
void show(char str_1[],int ct_1=9999)
{
    int i;
    for(i=0;i<ct_1&&str_1[i];i++) cout<<str_1[i];
    cout<<endl;
}
int main()
{
    STRING str;
    char str_1[]="hello world";
    set(str,str_1);
    show(str);//输出str字符串
    show(str,3);//输出str前三个字符
    show(str_1);//输出str_1
    show(str_1,3);//输出str_1前三个字符
    show("finished");
    return 0;
}*/

#include <iostream>
#include <string>
#define ESP 1e-6
using namespace std;

double check(double a,double b)
{
    if (b-0<ESP&&b-0>-ESP) throw string("error");
    return a/b;
}
int main()
{
   double a,b;
   START:
   cin>>a>>b;
   try
   {
       cout<<a<<"/"<<b<<"="<<check(a,b)<<endl;
   }
   catch(string str)
   {
       cout<<"error"<<endl;
       goto START;
   }
   return 0;
}

