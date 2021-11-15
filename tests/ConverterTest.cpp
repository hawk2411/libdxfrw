//
// Created by adwaha9 on 15.11.2021.
//

#include <memory>

#include "gtest/gtest.h"
#include "libdwgr.h"

class ConverterTest : public ::testing::Test {
protected:
    std::unique_ptr<dwgR> _dwgReader;
    const std::string test_file_name = "./data/SOT23_Stacks_2D_CC_Full.dxf";
protected:
    void SetUp() override {
        _dwgReader = std::make_unique<dwgR>(test_file_name.c_str());
    }

    void TearDown() override {
    }
};

TEST_F(ConverterTest, StartFunctinalParserReturnsNoError_test) {

    ASSERT_TRUE(_dwgReader->testReader());
}


