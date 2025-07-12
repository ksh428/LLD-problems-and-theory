#include<bits/stdc++.h>

using namespace std;
// you have a tool which analyses json data, but in youmain code your data source is xml
// you have a client which uses this tool to analyse json data, but it is not aware of the datatype which it gets

class XMLData{
    private:
    string val;
    public:
    XMLData() {};
    XMLData(string s)
    {
        val=s;
    }
    string getData()
    {
        return val;
    }
};
class DataTool{
    private:
    string jsondata;
    public:
    DataTool() {};
    DataTool(string jdata)
    {
        jsondata=jdata;
    }
    virtual void analyse()
    {
        cout<<"analysing json data";
    }
};
class Adapter:public DataTool{
    private:
    XMLData xmldata;
    public:
    Adapter(XMLData v)
    {
        xmldata=v;
    }
    void analyse() override
    {
        cout<<"converting xmldata to json data for use by base class";
        DataTool::analyse();
    }
};


class Client{
    public:
    void processdata(DataTool* tool)
    {
        tool->analyse();
    }
};

int main()
{
    XMLData data= XMLData("xml data");
    DataTool* tool=new Adapter(data);
    Client c=Client();
    c.processdata(tool);
}