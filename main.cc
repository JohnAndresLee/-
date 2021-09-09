#include<iostream>
#include<regex>
#include<iomanip>
#include"varhandle.hh"
#include"funchandle.hh"
#include"calculate.hh"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include<map>
#include"rerror.hh"
#include<utility>

using namespace std;

int main()
{
    map<string, string> m;
    map<string, pair<vector<string>, string>> func;
    while (true)
    {
        int n = 0;
        string str;
        smatch result;
        cout << "-->";

        getline(cin, str);

        regex regVar("Var ([a-zA-Z]+)=([a-zA-Z0-9_,?:\\+\\-\\*\\/\\.\\(\\)]+)");
        regex regFunc("Func ([a-zA-Z]+)(\\(([a-zA-Z,]+)\\))=([a-zA-Z0-9_,?:\\+\\-\\*\\/\\.\\(\\)]+)");
        regex regalterVar("([a-zA-Z]+)=([a-zA-Z0-9_,?:\\+\\-\\*\\/\\.\\(\\)]+)");
        regex regcalVar("([a-zA-Z]+)");
        regex regcalFunc("([a-zA-Z]+)(\\(([a-zA-Z0-9,]+)\\))");
        regex regcalexpres("([a-zA-Z0-9_,?:\\+\\-\\*\\/\\.\\(\\)]+)");
        regex regshowchar("SHOW VAR");
        regex regshowfunc("SHOW FUNC");
        regex regexit("exit");

        if (regex_match(str, regVar))
        {
            n = 1;
            regex_search(str, result, regVar);
        }
        else if (regex_match(str, regFunc)) {
            n = 2;
            regex_search(str, result, regFunc);
        }
        else if (regex_match(str, regcalVar)) {
            n = 3;
            regex_search(str, result, regcalVar);
        }
        else if (regex_match(str, regcalFunc)) {
            n = 4;
            regex_search(str, result, regcalFunc);
        }
        else if (regex_match(str, regcalexpres)) {
            n = 5;
            regex_search(str, result, regcalexpres);
        }
        else if (regex_match(str, regshowchar)) {
            n = 6;
        }
        else if (regex_match(str, regshowfunc)) {
            n = 7;
        }
        else if (regex_match(str, regexit)) {
            return 0;
        }
        else if (regex_match(str, regalterVar)) {
            n = 8;
            regex_search(str, result, regalterVar);
        }//改变之前定义变量的表达式
        else {
            cout << "Illegal Input!" << endl;
        }

        varhandle* fvar = new varhandle;
        calculate* fcal = new calculate;
        funchandle* ffunc = new funchandle;
        switch (n)
        {
        case 1:
        {
            fvar->vardefine(result.str(1), result.str(2),m);
            break;
        }
        case 2:
        {
            vector<string> vname;
            ffunc->splitStringToVect(result.str(3),vname,",");
            pair<vector<string>, string> p(vname, result.str(4));

            // map<string, pair<vector<string>, string>> func;

            pair<std::map<string, pair<vector<string>, string>>::iterator, bool> ret;
            ret = func.insert(pair<string, pair<vector<string>, string>>(result.str(1), p));
            if (!ret.second)
            {
                cout <<"ERROR:Func "<< result.str(1)<< " has been Defined" << endl;
            }
            else
            {
                cout << "Define Successfully!" << endl;
            }
            break;
        }
        case 3:
        {
            map<string, string>::iterator iter;
            bool flag = false;
            string expres;
            for (iter = m.begin(); iter != m.end(); iter++)
            {
                if (iter->first == result.str(0))
                {
                    expres = iter->second;
                    pair<string, bool> ret = fcal->simplify(m, func, expres);
                    if (ret.second)
                    {
                        pair<float, bool> r = fcal->calvalue(expres);
                        if (r.second)
                        {
                            float answer = r.first;
                            cout << "-->" << fixed << setprecision(6) << answer << endl;
                            flag = true;
                        }
                        else
                        {
                            cout << "ERROR:Wrong Expression" << endl;
                        }
                        break;
                    }
                    else
                    {
                        cout << ret.first << endl;
                    }
                }
                else
                    continue;
            }
            if (!flag)
            {
                throwerror(7);
                cout << result.str(0) << endl;
            }
          break;
        }
        case 4:
        {
            string expres;
            vector<string> assign;
            ffunc->splitStringToVect(result.str(3), assign, ",");
            if (ffunc->func_assign(func, assign, result.str(1), expres))
            {
                while (expres.find('?') != expres.npos)
                {
                    expres=fcal->initialjudge(m, func, expres).first;
                }

                pair<string, bool> ret=fcal->simplify(m, func, expres);
                if (ret.second)
                {
                    pair<float, bool> r = fcal->calvalue(expres);
                    if (r.second)
                    {
                        float answer = r.first;
                        cout << "-->" << fixed << setprecision(6) << answer << endl;
                    }
                    else
                    {
                        cout << "ERROR:Wrong Expression" << endl;
                    }
                }
                else
                {
                    cout << ret.first << endl;
                }
            }
            break;
        }
        case 5:
        {
            string expres = result.str(0);
            
            while (expres.find('?')!=expres.npos)
            {
                fcal->initialjudge(m,func,expres);
            }

            pair<string, bool> ret=fcal->simplify(m, func,expres);
            if (ret.second)
            {
                pair<float, bool> r = fcal->calvalue(expres);
                if (r.second)
                {
                    float answer = r.first;
                    cout << "-->" << fixed << setprecision(6) << answer << endl;
                }
                else
                {
                    cout << "ERROR:Wrong Expression" << endl;
                }
            }
            else
            {
                cout << ret.first << endl;
            }
            break;
        }
        case 6:
        {
            fvar->showvar(m);
            break;
        }
        case 7:
        {
            ffunc->showfunc(func);
            break;
        }
        case 8:
        {
            map<string, string>::iterator iter;
            bool flag = false;
            string expres;
            for (iter = m.begin(); iter != m.end(); iter++)
            {
                if (iter->first == result.str(1))
                {
                    m.erase(iter);
                    m.insert(pair<string, string>(result.str(1), result.str(2)));
                    flag = true;
                    cout << "Alter Successfully!"<<endl;
                    break;
                }
                else
                    continue;
            }
            if (!flag)
            {
                throwerror(7);
                cout << result.str(0) << endl;
            }
            break;
        }
        default:break;
        }
    }
    return 0;
}