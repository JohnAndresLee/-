#include"funchandle.hh"
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"calculate.hh"
#include<map>

using namespace std;

int funchandle::funcfileread(){
    return 0;
}

int funchandle::funcfilewrite(){
    return 0;

}

void funchandle::showfunc(map<string, pair<vector<string>, string>> &func)
{
    map<string, pair<vector<string>, string>>::iterator iter;
    for (iter = func.begin(); iter != func.end(); iter++)
    {
        cout << iter->first<<"("<<iter->second.first[0];
        for (int i = 1; i < iter->second.first.size(); ++i)
        {
            cout <<","<<iter->second.first[i];
        }
        cout << ")=" << iter->second.second << endl;
    }
    return;
}

int frecursion()
{
    return 0;
}

int funchandle::splitStringToVect(const string& srcStr, vector<string>& destVect, const string& strFlag)
{
    int pos = srcStr.find(strFlag, 0);
    int startPos = 0;
    int splitN = pos;
    string lineText(strFlag);
    while (pos > -1)
    {
        lineText = srcStr.substr(startPos, splitN);
        startPos = pos + 1;
        pos = srcStr.find(strFlag, pos + 1);
        splitN = pos - startPos;
        destVect.push_back(lineText);
    }
    lineText = srcStr.substr(startPos, srcStr.length() - startPos);
    destVect.push_back(lineText);
    return destVect.size();
}

int funchandle::splitStringToVectextra(const string& srcStr, vector<string>& destVect)
{
    int bracketnum = 0;
    int len = srcStr.length();
    for (int i = 0; i < len; ++i)
    {
        string temp;
        temp += '(';
        while (i<len)
        {
            if (srcStr[i] == '(')
                bracketnum++;
            else if (srcStr[i] == ')')
                bracketnum--;
            else if (srcStr[i] == ',')
            {
                if (bracketnum == 0)
                {
                    temp += ')';
                    destVect.push_back(temp);
                    break;
                }
            } 
            temp += srcStr[i];
            i++;
        }
        if (i == len)
        {
            temp += ')';
            destVect.push_back(temp);
        }
    }
    return destVect.size();
}

bool funchandle::judge_isRight(string str)
{
    return true;

}

bool func_exist(string str)
{
    return false;
}

bool funchandle::func_assign(map<string, pair<vector<string>, string>>& func,vector<string> assign,string funcname,string &dest)
{
    bool flag = false;
    map<string, pair<vector<string>, string>>::iterator iter;
    for (iter = func.begin(); iter != func.end(); iter++)
    {
        if (funcname == iter->first)
        {
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        cout << "ERROR:Func " << funcname << " Not Exist" << endl;
        return false;
    }
    else
    {
        if (iter->second.first.size() != assign.size())
        {
            cout << "ERROR:Variable Number Not Match" << endl;
            cout << "Func " << iter->first << " Should have " << iter->second.first.size() << " Variable(s)" << endl;
            return false;
        }
        else
        {
            string expres = iter->second.second;
            for (int i = 0; i < assign.size(); ++i)
            {
                dest= '(';
                expres = replace_all(expres,iter->second.first[i], assign[i]);
                dest += expres;
                dest += ')';
            }
            return true;
        }
    }
}

string& funchandle::replace_all(string& str, const   string& old_value, const   string& new_value)
{
    if (old_value == new_value)
        return str;
    else 
    {
        while (true) {
            string::size_type   pos(0);
            if ((pos = str.find(old_value)) != string::npos)
                str.replace(pos, old_value.length(), new_value);
            else
                break;
        }
        return  str;
    }
}
