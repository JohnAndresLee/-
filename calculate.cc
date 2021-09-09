#include <stdio.h>
#include <stdlib.h>
#include<string>
#include"calculate.hh"
#include"varhandle.hh"
#include<vector>
#include"funchandle.hh"
#include<time.h>
#include<iostream>

using namespace std;

clock_t start, stop;
#define min 1e-6

struct opstack {
    char data[MaxSize];//存储操作符 
    int top;//指向栈顶的指针 
}op;
//数值栈
struct ststack {
    float data[MaxSize];//存储操作符数 
    int top;//指向栈顶的指针 
}st;
//符号栈

void throwerror1(int signal)
{
    switch (signal)
    {
    case 0:cout << "ERROR:Variable "<<endl; break;//变量名命名错误
    case 1:cout << "ERROR:Primary Expected" << endl; break;//运算符错位
    case 2:cout << "ERROR:Unexpected Token" << endl; break;//多余运算符
    case 3:cout << "ERROR:Invalid Characters" << endl; break;//出现非法字符
    case 4:cout << "ERROR:Too Many Right Parentheses" << endl;break;//右括号多于左括号
    case 5:cout << "ERROR:Unexpected Token" << endl; break;//变量名命名错误
    case 6:cout << "ERROR:Divide By Zero" << endl; break;//除零错误
    case 7:cout << "ERROR:Undefined Variable "; break;//未定义变量
    case 8:cout << "ERROR:Multiple Sign Expected" << endl; break;//乘号缺失
    default: break;
    }
    return;
}
//将算术表达式exp转换为后缀表达式postexp
string calculate::transform(string midexp) {
    char postexp[202] = {'\0'};
    char ch;
    int i = 0, j = 0; //i扫描exp的下标,j扫描postexp的下标
    op.top = -1;
    ch = midexp[i]; 
    i++;
    while (ch != '\0')
    {
        switch (ch) {
        case '(':    //左括号
            op.top++; op.data[op.top] = ch;
            break;
        case ')':   //右括号
            while (op.data[op.top] != '(')//若遇到右括弧“）”，则连续出栈输出，直到遇到左括弧“（”为止。其中，左括弧出栈但是并不输出 
            {
                postexp[j] = op.data[op.top]; j++;
                op.top--;
                if (op.top <= -1)
                {
                    throwerror1(4);
                    return "BRACKET ERROR!";
                }
            }
            op.top--;
            break;
        case '+':   //为'+'或'-'时，其优先级不大于栈顶任何运算符的优先级，直到')'
        case '-':
            while (op.top != -1 && op.data[op.top] != '(')
            {
                postexp[j] = op.data[op.top]; j++;
                op.top--;
            }
            op.top++; op.data[op.top] = ch;
            break;
        case '*':
        case '/':  //为'*'或'/'时，其优先级不大于栈顶为'*'或'/'的优先级，直到')'
            while (op.top != -1 && op.data[op.top] != '('
                && (op.data[op.top] == '*' || op.data[op.top] == '/')) {
                postexp[j] = op.data[op.top]; j++;
                op.top--;
            }
            op.top++; op.data[op.top] = ch;
            break;
        case ' ': break;  //过滤空格
        default:
        {
                while ((ch >= '0' && ch <= '9')||ch=='.')
                {
                    postexp[j] = ch; j++;
                    ch = midexp[i]; i++;
                }
                i--;
                postexp[j] = '#'; j++;
                //postexp[j]=' '; j++; //用空格标识一个数值串结束
        }
        }
        ch = midexp[i]; i++;
    }

    while (op.top != -1) { //此时，exp扫描完毕，栈不空时出栈并存放到postexp中
        if (op.data[op.top] == '(' || op.data[op.top] == ')')
            return "BRACKET ERROR!";
        else
        {
            postexp[j] = op.data[op.top]; j++;
            op.top--;
        }
    }
    postexp[j] = '\0'; //给postexp表达式添加结束标识
    
    return postexp;
}

//对后缀表达式postexp求值
pair<float,bool> calculate::calvalue(string midexp) 
{
    string postexp=transform(midexp);
    if (postexp == "BRACKET ERROR!")
        return make_pair(-1,false);//左右括号不匹配返回-1
    float d;
    char ch;
    int i = 0;
    st.top = -1;
    ch = postexp[i]; i++;
    while (ch != '\0')
    {
        switch (ch) {
        case '+':
        {
            if (st.top >= 1)
            {
                st.data[st.top - 1] = st.data[st.top - 1] + st.data[st.top];//遇到操作符就弹出两个数 并将结果进栈 
                st.top--; break;
            }
            else
                return make_pair(2,false);
        }
        case '-':
        {
            if (st.top >= 1)
            {
                st.data[st.top - 1] = st.data[st.top - 1] - st.data[st.top];
                st.top--; break;
            }
            else
                return make_pair(2,false);
        }
        case '*': 
        {
            if (st.top >= 1)
            {
                st.data[st.top - 1] = st.data[st.top - 1] * st.data[st.top];
                st.top--; break;
            }
            else
            {
                return make_pair(0,false);//乘号错误返回0
            }
        }
        case '/':
            if (st.top >= 1)
            {
                if (st.data[st.top] >= min)
                    st.data[st.top - 1] = st.data[st.top - 1] / st.data[st.top];
                else
                {
                    throwerror1(6);
                    return make_pair(0, false);
                }
            }
            else
            {
                return make_pair(1, false);//乘号错误返回1
            }
            st.top--; break;
        default:
            d = 0;
            double dd = 1;
            bool flag = true;
            while ((ch >= '0' && ch <= '9')||ch=='.')//遇到操作数就进栈直到#为止 
            {
                if (ch == '.')
                {
                    flag = false;
                    ch = postexp[i]; i++;
                }
                else 
                {
                    if (flag)
                    {
                        d = 10 * d + ch - '0';
                        ch = postexp[i]; i++;
                    }
                    else
                    {
                        dd = ((ch - '0')*dd)/ 10  ;
                        ch = postexp[i]; i++;
                    }
                }
            }
            if (!flag)
            {
                d = d + dd;
            }
            st.top++;
            st.data[st.top] = d;

        }
        ch = postexp[i]; i++;
    }
    return make_pair(st.data[st.top],true);//输出栈顶元素就是结果 
}



//此函数的目的是将变量化到最简（数字），若化不到则返回false，并报错
//需要考虑的是变量定义中存在其他变量或者是函数
//会不会出现Var x=x+1这类死循环
//对表达式进行处理    x+a+x*a+a/3
// 
//f(f(1,a),f(1,a))的问题！！！
pair<string, bool> calculate::simplify(map<string, string>& m, map<string, pair<vector<string>, string>>& func,string& expres)
{
 /*   pair<string, bool> pai;
    pai = init(expres);
    if (!pai.second)
        return make_pair(pai.first, false);
    else {*/
        funchandle* ffunc = new funchandle;
        int i = expres.length();
        pair<int, bool> p = judgeexpre(expres);
        start = clock();
        while (!p.second)
        {
            i = expres.length();
            int pos = p.first;//第一个字母的位置
            int n = 0;
            string varfunc;
            while ((expres[pos] >= 65 && expres[pos] <= 90) || (expres[pos] >= 97 && expres[pos] <= 122))
            {
                varfunc += expres[pos];
                pos++;
                n++;
            }
            if (expres[pos] == '(')
            {
                int bracketnum = 1;
                int num = 1;
                string value = varfunc;
                string tempp;
                value += '(';
                pos++;
                while (bracketnum != 0)
                {
                    if (expres[pos] == ',')
                    {
                        if (bracketnum == 1)
                            num++;
                        tempp += expres[pos];
                        value += expres[pos];
                    }
                    else if (expres[pos] == '(')
                    {
                        bracketnum++;
                        tempp += expres[pos];
                        value += expres[pos];
                    }
                    else if (expres[pos] == ')')
                    {
                        bracketnum--;
                        if (bracketnum != 0)
                        {
                            tempp += expres[pos];
                            value += expres[pos];
                        }
                        else
                            value += expres[pos];
                    }
                    else
                    {
                        tempp += expres[pos];
                        value += expres[pos];
                    }
                    if (pos == i && expres[pos] != ')')
                        return make_pair("ERROR:Parentheses Not Match", false);
                    else
                    {
                        pos++;
                        n++;
                    }
                }
                //           value+= expres[pos];
                pair<string, bool> temp = funcexpress(varfunc, num, func);//获得函数的表达式
                if (temp.second)
                {
                    vector<string> assign;

                    ffunc->splitStringToVectextra(tempp, assign);

                    ffunc->func_assign(func, assign, varfunc, temp.first);
                    replace_all(expres, value, temp.first);
                }
                else
                {
                    return temp;
                }
            }
            else
            {
                pair<string, bool> temp = varexpress(varfunc, m);

                pair<string, bool> kl = simplify(m,func,temp.first);

                string answer;
                if (kl.second)
                {
                    pair<float, bool> r0 = calvalue(kl.first);
                    if (!r0.second)
                    {
                        return make_pair("ERROR:Unavailable Variables", false);
                    }
                }

                if (temp.second)
                {
                    replace_all(expres, varfunc, temp.first);
                }
                else
                {
                    return temp;
                }
            }
            p = judgeexpre(expres);
       /*     stop = clock();
            double duration = (double)(stop - start) / CLOCKS_PER_SEC;
            if (duration > 2)
            {
                return make_pair("ERROR:RUNTIME ERROR", false);
            }*/
        }
        return make_pair(expres, true);
  //  }
}

//想法是通过这个函数检查一下能否计算，以及三目运算的实现
pair<string, bool> calculate::initialjudge(map<string, string>& m, map<string, pair<vector<string>, string>>& func, string& expres)
{

    int n = expres.length();

    for (int i = 0; i < n; ++i)//括号的位置分析
    {
        if (expres[i] == '?')
        {
            int quespos = i;//记录？的位置
            string ex1, ex2, ex3;

            int bracketnum1 = 0;
            i--;
            while (i >= 0)
            {
                if (expres[i] == '(')
                {
                    if (bracketnum1 == 0)
                    {
                        break;
                    }
                    else {
                        bracketnum1++;
                        ex1 += expres[i];
                        i--;
                    }
                }
                else if (expres[i] == ')')
                {
                    bracketnum1--;
                    ex1 += expres[i];
                    i--;
                }
                else if (bracketnum1 == 0 && (expres[i] == '+' || expres[i] == '-'))
                    break;
                else
                {
                    ex1 += expres[i];
                    i--;
                }
            }
            int begin=i+1;
            ex1 = reverse(ex1);

            i = quespos+1;
            int bracketfuck = 0;
            int numq = 0;
            while (expres[i] != ':'||(expres[i] == ':'&&numq!=0))
            {
                if (expres[i] == '(')
                {
                    bracketfuck++;
                    ex2 += expres[i];
                    i++;
                }
                else if (expres[i] == ')' && bracketfuck != 0)
                {
                    bracketfuck--;
                    ex2 += expres[i];
                    i++;
                }
                else if (expres[i] == '?')
                {
                    numq++;
                    ex2 += expres[i];
                    i++;
                }
                else if (expres[i] == ':')
                {
                    numq--;
                    ex2 += expres[i];
                    i++;
                }
                else
                {
                    ex2 += expres[i];
                    i++;
                }
            }

            i++;
            int bracketnum2 = 0;
            while (i < n)
            {
                if (expres[i] == '(')
                {
                    bracketnum2++;
                    ex3 += expres[i];
                    i++;
                }
                else if (expres[i] == ')'&&bracketnum2!=0)
                {
                    bracketnum2--;
                    ex3 += expres[i];
                    i++;
                }
                else if (bracketnum2 == 0 && (expres[i] == '+' || expres[i] == '-'||expres[i]==')'))
                    break;
                else
                {
                    ex3 += expres[i];
                    i++;
                }
            }
            int mend = i;

            if (simplify(m, func,ex1).second)
            {
                pair<float, bool> r = calvalue(ex1);
                if (r.second)
                {
                    float answer = r.first;
                    if (answer < min)//若ex1结果为0则将表达式全部替换为ex2
                    {
                        expres.replace(begin, mend - begin, ex3);
                    }
                    else//否则替换为ex3
                    {
                        expres.replace(begin, mend, ex2);
                        expres += ')';
                    }
                }
                else
                    return make_pair("ERROR:Wrong Expression", false);
            }
            n = expres.length();
        }
    }
    return make_pair(expres, true);
}

string calculate::reverse(string str)
{
    string r;
    int n = str.length();
    for (int i = n - 1; i >= 0; --i)
    {
        r += str[i];
    }
    return r;
}

string& calculate::replace_all(string& str, const string& old_value, const string& new_value)
{
    
    while (true) {
        string::size_type   pos(0);
        if ((pos = str.find(old_value)) != string::npos)
            str.replace(pos, old_value.length(), new_value);
        else
            break;
 /*       stop = clock();
        double duration = (double)(stop - start) / CLK_TCK;
        if (duration > 2)
        {
            return str;
        }*/
    }
    return  str;
}

pair<int,bool> calculate::judgeexpre(string expres)
{
    int n = expres.length();
    for (int i = 0; i < n; ++i)
    {
        if ((expres[i] >= 65 && expres[i] <= 90) || (expres[i] >= 97 && expres[i] <= 122))
            return make_pair(i,false);
    }
    return make_pair(0,true);
}

pair<string, bool> calculate::funcexpress(string funcname,int fn,map<string, pair<vector<string>, string>>& func)
{
    map<string, pair<vector<string>, string>>::iterator iter;
    bool flag = false;
    string expres="(";
    for (iter = func.begin(); iter != func.end(); iter++)
    {
        if (iter->first ==funcname)
        {
            if (fn == iter->second.first.size())
            {
                expres+= iter->second.second;
                flag = true;
                break;
            }
            else
            {
                flag = false;
                return make_pair("Variable Number Error", flag);
            }
        }
        else
            continue;
    }
    if (!flag)
        return make_pair("Func Not Exist", flag);
    else
    {
        expres += ")";
        return make_pair(expres, flag);
    }
}

pair<string,bool>  calculate::init(string& expres)
{
    int n = expres.length();
    int pos = 0;
    for (int i = 0; i < n; ++i)
    {
        int plus = 0;
        int minus = 0;
        if (expres[i] == '+' || expres[i] == '-')
        {
            pos = i;
            while (expres[i] == '+' || expres[i] == '-')
            {
                if (expres[i] == '+')
                    plus++;
                else
                    minus++;
                i++;
                if (i >= n)
                {
                    return make_pair("ERROR:WRONG EXPRESSION", false);
                    break;
                }
            }
            int k = plus - minus;
            if (k>0)
            {
                expres.replace(pos,i-pos,"+");
                n = expres.length();
            }
            else if (k== 0)
            {
                return make_pair("ERROR:WRONG EXPRESSION", false);
            }
            else
            {
                if (k % 2 == 0)
                {
                    expres.replace(pos, i - pos, "+");
                    n = expres.length();
                }
                else
                {
                    expres.replace(pos, i - pos, "-");
                    n = expres.length();
                }
            }  
        }
    }
    return make_pair(expres, true);
}

pair<string,bool> calculate::varexpress(string varname, map<string, string>& m)
{
    map<string, string>::iterator iter;
    bool flag = false;
    string expres="(";
    for (iter = m.begin(); iter != m.end(); iter++)
    {
        if (iter->first == varname)
        {
            expres+= iter->second;
            flag = true;
            break;
        }
        else
            continue;
    }
    if (!flag)
    {
        string re = "ERROR:Variable ";
        re += varname;
        re += " Not Exist";
        return make_pair(re, flag);
    }
    else
    {
        expres += ")";
        return make_pair(expres, flag);
    }
}