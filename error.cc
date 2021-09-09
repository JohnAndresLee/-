#include"rerror.hh"
#include<iostream>

using namespace std;

void throwerror(int signal)
{
    switch (signal)
    {
    case 0:cout << "ERROR:Variable ";break;//变量名命名错误
    case 1:cout << "ERROR:Primary Expected" << endl; break;//运算符错位
    case 2:cout << "ERROR:Unexpected Token" << endl; break;//多余运算符
    case 3:cout << "ERROR:Invalid Characters" << endl;break;//出现非法字符
    case 4:cout << "ERROR:Too Many Right Parentheses" << endl; break;//右括号多于左括号
    case 5:cout << "ERROR:Unexpected Token" << endl; break;//变量名命名错误
    case 6:cout << "ERROR:Divide By Zero" << endl; break;//除零错误
    case 7:cout << "ERROR:Undefined Variable "; break;//未定义变量
    case 8:cout << "ERROR:Multiple Sign Expected" << endl; break;//乘号缺失
    default: break;
    }
    return;
}