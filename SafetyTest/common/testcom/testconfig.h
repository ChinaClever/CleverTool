#ifndef TESTCONFIGFILE_H
#define TESTCONFIGFILE_H
#include "configbase.h"

class TestConfig : public ConfigBase
{
    TestConfig();
public:
    static TestConfig *bulid();
    QString getPrefix(){return "test";}
};

#endif // TESTCONFIGFILE_H
