#include<string>
#include<utility>
#include<map>
#include<vector>

using namespace std;

#define MaxSize 100

class calculate
{
public:

pair<float, bool> calvalue(string postexp);

string transform(string midexp);

pair<string, bool> simplify(map<string, string>& m,map<string, pair<vector<string>, string>>& func,string& expres);

pair<string, bool> initialjudge(map<string, string>& m, map<string, pair<vector<string>, string>>& func, string& expres);
//�뷨��ͨ������������һ���ܷ���㣬�Լ���Ŀ�����ʵ��

private:
	pair<string, bool> init(string& expres);
	string reverse(string str);
	pair<int, bool> judgeexpre(string expres);
	string& replace_all(string& str, const string& old_value, const string& new_value);
	pair<string, bool> funcexpress(string funcname, int fn, map<string, pair<vector<string>, string>>& func);
	pair<string, bool> varexpress(string varname, map<string, string>& m);

};
