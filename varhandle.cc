#include<iostream>
#include<iomanip>
#include"varhandle.hh"
#include<fstream>
#include<string>
#include"rerror.hh"
#include"calculate.hh"
#include<map>

using namespace std;

void varhandle::showvar(map<string, string> &m)
{
    map<string,string>::iterator iter;
    for (iter = m.begin(); iter != m.end(); iter++)
    {
        cout << iter->first << "=" << iter->second << endl;
    }
    return;
}

void varhandle::vardefine(string varname,string varexpres,map<string,string> &m)
{
    pair<std::map<string, string>::iterator, bool> ret;
    ret = m.insert(pair<string, string>(varname, varexpres));
    if(!ret.second)
    {
        throwerror(0);//抛出变量命名错误
        cout << varname<<" has been Defined"<<endl;
        return;
    }
    else
    {
        cout << "Define Successfully!" << endl;
        return;
    }
    return;
}
