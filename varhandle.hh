#include<string>
#include<map>
#include<utility>

using namespace std;

class varhandle
{
private:
public:
    void vardefine(string varname,string varexpres,map<string,string> &m);
    void showvar(map<string, string> &m);
};
