/** This is more demonstration of how to use the IAbstract, less a test. But could still be a worthy playground.*/
#include "toolib/type/iabstract.h"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
#include <string>

namespace too = mb::too;

class IAbstractTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

    class CTest : public too::iabstract {
    public:
        ~CTest() override = default;

        //! Parameter documentation, also for setParameter().
        /** ... (in the real world it is extremely to provide a thorough documention of allowed name-type pairs)*/
        bool getParameter(const char* name, too::TOutBuffer value) const override {
            if (!name || !value)
                return false;
            std::string n(name);
            if (n == "i")
                *reinterpret_cast<int*>(value) = m_i;
            else if (n == "d")
                *reinterpret_cast<double*>(value) = m_d;
            else if (n == "s")
                *reinterpret_cast<std::string*>(value) = m_s;
            else
                return false;
            return true;
        }

        //! Cf. getParameter()
        bool setParameter(const char* name, const too::TInBuffer value) override {
            if (!name || !value)
                return false;
            std::string n(name);
            if (n == "i")
                m_i = *reinterpret_cast<int*>(value);
            else if (n == "d")
                m_d = *reinterpret_cast<double*>(value);
            else if (n == "s")
                m_s = *reinterpret_cast<std::string*>(value);
            else
                return false;
            return true;
        }

        //! In the real world don't forget to provide a detailed documentation like for getParameter().
        bool execute(
            const char* command, const too::TInBuffer params[] = nullptr, too::TOutBuffer retvalue = nullptr) override {
            if (!command)
                return false;
            std::string c(command);
            if (c == "reset")
                reset();
            else if (c == "run")
                run();
            else if (c == "calcSth") {
                SCalcSthParams p;
                if (!ObtainParamsFromAbstract(params, p))
                    return false;
                *reinterpret_cast<std::string*>(retvalue) = CalcSth(p);
            } else
                return false;
            return true;
        }

    private:
        int m_i = 0;
        double m_d = 0.0;
        //! Be careful: This is only an example of a more complex type. You shouldn't use std::string over binary
        //! boundaries.
        std::string m_s;

        void reset() {
            m_i = 0;
            m_d = 0.0;
            m_s.clear();
        }

        void run() const {
            std::cout << "\nCTest::run() called\n";
        }

        struct SCalcSthParams {
            int i = 0;
            std::string s;
        };

        bool ObtainParamsFromAbstract(const too::TInBuffer params[], SCalcSthParams& ret) const {
            if (!params || !params[0] || !params[1])
                return false;
            ret.i = *reinterpret_cast<int*>(params[0]);
            ret.s = *reinterpret_cast<std::string*>(params[1]);
            return true;
        }

        std::string CalcSth(const SCalcSthParams& p) const {
            std::string ret;
            std::stringstream sconv;
            sconv << m_i * p.i;
            ret += sconv.str();
            ret += ";";
            ret += "\"";
            ret += m_s + p.s;
            ret += "\"";
            return ret;
        }
    };

    CTest m_TestObj;
};

TEST_F(IAbstractTest, GetParameterInitially) {
    int i = 5;
    EXPECT_TRUE(m_TestObj.getParameter("i", &i));
    EXPECT_EQ(0, i);
    double d = 5.0;
    EXPECT_TRUE(m_TestObj.getParameter("d", &d));
    EXPECT_EQ(0.0, d);
    std::string s("non-empty");
    EXPECT_TRUE(m_TestObj.getParameter("s", &s));
    EXPECT_EQ("", s);
}

TEST_F(IAbstractTest, GetParameterWrong) {
    int i = 5;
    EXPECT_FALSE(m_TestObj.getParameter("xy", &i));
    EXPECT_EQ(5, i);
    // try crash
    //    EXPECT_DEATH(m_TestObj.GetParameter("s", &i), "");
    // ... or temporarily via
    //    m_TestObj.GetParameter("s", &i);
}

TEST_F(IAbstractTest, SetGetParameter) {
    int i = 10;
    EXPECT_TRUE(m_TestObj.setParameter("i", &i));
    double d = 10.5;
    EXPECT_TRUE(m_TestObj.setParameter("d", &d));
    std::string s("Hello World!");
    EXPECT_TRUE(m_TestObj.setParameter("s", &s));
    i = 5;
    EXPECT_TRUE(m_TestObj.getParameter("i", &i));
    EXPECT_EQ(10, i);
    d = 5.0;
    EXPECT_TRUE(m_TestObj.getParameter("d", &d));
    EXPECT_EQ(10.5, d);
    s = "non-empty";
    EXPECT_TRUE(m_TestObj.getParameter("s", &s));
    EXPECT_EQ("Hello World!", s);
}

TEST_F(IAbstractTest, SetParameterWrong) {
    int i = 5;
    EXPECT_FALSE(m_TestObj.setParameter("xy", &i));
    EXPECT_EQ(5, i);
    // try crash
    //    EXPECT_DEATH(m_TestObj.SetParameter("s", &i), "");
    // ... or temporarily via
    //    m_TestObj.SetParameter("s", &i);
}

TEST_F(IAbstractTest, ExecuteWithoutPara) {
    int i = 10;
    m_TestObj.setParameter("i", &i);
    double d = 10.5;
    m_TestObj.setParameter("d", &d);
    std::string s("Hello World!");
    m_TestObj.setParameter("s", &s);
    EXPECT_TRUE(m_TestObj.execute("reset"));
    i = 5;
    EXPECT_TRUE(m_TestObj.getParameter("i", &i));
    EXPECT_EQ(0, i);
    d = 5.0;
    EXPECT_TRUE(m_TestObj.getParameter("d", &d));
    EXPECT_EQ(0.0, d);
    s = "non-empty";
    EXPECT_TRUE(m_TestObj.getParameter("s", &s));
    EXPECT_EQ("", s);
    EXPECT_TRUE(m_TestObj.execute("run"));
}

TEST_F(IAbstractTest, ExecuteWithParaAndRet) {
    int i = 10;
    m_TestObj.setParameter("i", &i);
    std::string s("Hello World!");
    m_TestObj.setParameter("s", &s);
    std::string out;
    int ip = 2;
    std::string sp = " And once more: Hello World!";
    void* param[] = {&ip, &sp};
    EXPECT_TRUE(m_TestObj.execute("calcSth", param, &out));
    EXPECT_EQ("20;\"Hello World! And once more: Hello World!\"", out);
}
