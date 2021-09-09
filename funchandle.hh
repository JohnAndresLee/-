#include<vector>
#include<string>
#include<map>
using namespace std;

class funchandle
{
private:
    int funcfileread();
    int funcfilewrite();
    string& replace_all(string& str, const   string& old_value, const   string& new_value);
public:
    bool func_assign(map<string, pair<vector<string>, string>>& func, vector<string> assign, string funcname,string&dest);
    void showfunc(map<string, pair<vector<string>, string>>& func);
    int splitStringToVect(const string& srcStr, vector<string>& destVect, const string& strFlag);
    int splitStringToVectextra(const string& srcStr, vector<string>& destVect);
    bool judge_isRight(string str);
};
