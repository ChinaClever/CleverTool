#ifndef TESTCONFIGFILE_H
#define TESTCONFIGFILE_H
#include "configbase.h"

enum {
    Test_Function, // 功能测试
    Test_Start,
    Test_Pause,
    Test_Continue,
    Test_Over,

    Test_Result_Wid,
    Test_Items_Wid,
    Test_Datas_Wid
};

class TestConfig : public ConfigBase
{
    TestConfig();
public:
    static TestConfig *bulid();
    QString getPrefix(){return "test";}
};

#endif // TESTCONFIGFILE_H
