/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "testconfig.h"

TestConfig::TestConfig()
{
    item = new sTestConfigItem();
}

TestConfig *TestConfig::bulid()
{
    static TestConfig* sington = nullptr;
    if(sington == nullptr)
        sington = new TestConfig();
    return sington;
}

