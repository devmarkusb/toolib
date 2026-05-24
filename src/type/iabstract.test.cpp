/** This is more demonstration of how to use the IAbstract, less a test. But could still be a worthy playground.*/
#include "mb/toolib/type/iabstract.h"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
#include <string>

class IAbstractTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    class CTest : public too::Iabstract {
    public:
        ~CTest() override = default;

        //! Parameter documentation, also for setParameter().
        /** ... (in the real world it is extremely to provide a thorough documention of allowed name-type pairs)*/
        bool get_parameter(const char* name, too::TOutBuffer value) const override {
            if (!name || !value)
                return false;
            const std::string n(name);
            if (n == "i")
                *reinterpret_cast<int*>(value) = m_i_;
            else if (n == "d")
                *reinterpret_cast<double*>(value) = m_d_;
            else if (n == "s")
                *reinterpret_cast<std::string*>(value) = m_s_;
            else
                return false;
            return true;
        }

        //! Cf. getParameter()
        bool set_parameter(const char* name, const too::TInBuffer value) override {
            if (!name || !value)
                return false;
            const std::string n(name);
            if (n == "i")
                m_i_ = *reinterpret_cast<int*>(value);
            else if (n == "d")
                m_d_ = *reinterpret_cast<double*>(value);
            else if (n == "s")
                m_s_ = *reinterpret_cast<std::string*>(value);
            else
                return false;
            return true;
        }

        //! In the real world don't forget to provide a detailed documentation like for getParameter().
        bool execute(
            const char* command, const too::TInBuffer params[] = nullptr, too::TOutBuffer retvalue = nullptr) override {
            if (!command)
                return false;
            const std::string c(command);
            if (c == "reset")
                reset();
            else if (c == "run")
                run();
            else if (c == "calcSth") {
                SCalcSthParams p;
                if (!obtain_params_from_abstract(params, p))
                    return false;
                *reinterpret_cast<std::string*>(retvalue) = calc_sth(p);
            } else
                return false;
            return true;
        }

    private:
        int m_i_ = 0;
        double m_d_ = 0.0;
        //! Be careful: This is only an example of a more complex type. You shouldn't use std::string over binary
        //! boundaries.
        std::string m_s_;

        void reset() {
            m_i_ = 0;
            m_d_ = 0.0;
            m_s_.clear();
        }

        static void run() {
            std::cout << "\nCTest::run() called\n";
        }

        struct SCalcSthParams {
            int i = 0;
            std::string s;
        };

        static bool obtain_params_from_abstract(const too::TInBuffer params[], SCalcSthParams& ret) {
            if (!params || !params[0] || !params[1])
                return false;
            ret.i = *reinterpret_cast<int*>(params[0]);
            ret.s = *reinterpret_cast<std::string*>(params[1]);
            return true;
        }

        [[nodiscard]] std::string calc_sth(const SCalcSthParams& p) const {
            std::string ret;
            std::stringstream sconv;
            sconv << m_i_ * p.i;
            ret += sconv.str();
            ret += ";";
            ret += "\"";
            ret += m_s_ + p.s;
            ret += "\"";
            return ret;
        }
    };

    CTest m_test_obj_;
};

TEST_F(IAbstractTest, GetParameterInitially) {
    int i = 5;
    EXPECT_TRUE(m_test_obj_.get_parameter("i", &i));
    EXPECT_EQ(0, i);
    double d = 5.0;
    EXPECT_TRUE(m_test_obj_.get_parameter("d", &d));
    EXPECT_EQ(0.0, d);
    std::string s("non-empty");
    EXPECT_TRUE(m_test_obj_.get_parameter("s", &s));
    EXPECT_EQ("", s);
}

TEST_F(IAbstractTest, GetParameterWrong) {
    int i = 5;
    EXPECT_FALSE(m_test_obj_.get_parameter("xy", &i));
    EXPECT_EQ(5, i);
    // try crash
    //    EXPECT_DEATH(m_TestObj.GetParameter("s", &i), "");
    // ... or temporarily via
    //    m_TestObj.GetParameter("s", &i);
}

TEST_F(IAbstractTest, SetGetParameter) {
    int i = 10;
    EXPECT_TRUE(m_test_obj_.set_parameter("i", &i));
    double d = 10.5;
    EXPECT_TRUE(m_test_obj_.set_parameter("d", &d));
    std::string s("Hello World!");
    EXPECT_TRUE(m_test_obj_.set_parameter("s", &s));
    i = 5;
    EXPECT_TRUE(m_test_obj_.get_parameter("i", &i));
    EXPECT_EQ(10, i);
    d = 5.0;
    EXPECT_TRUE(m_test_obj_.get_parameter("d", &d));
    EXPECT_EQ(10.5, d);
    s = "non-empty";
    EXPECT_TRUE(m_test_obj_.get_parameter("s", &s));
    EXPECT_EQ("Hello World!", s);
}

TEST_F(IAbstractTest, SetParameterWrong) {
    int i = 5;
    EXPECT_FALSE(m_test_obj_.set_parameter("xy", &i));
    EXPECT_EQ(5, i);
    // try crash
    //    EXPECT_DEATH(m_TestObj.SetParameter("s", &i), "");
    // ... or temporarily via
    //    m_TestObj.SetParameter("s", &i);
}

TEST_F(IAbstractTest, ExecuteWithoutPara) {
    int i = 10;
    m_test_obj_.set_parameter("i", &i);
    double d = 10.5;
    m_test_obj_.set_parameter("d", &d);
    std::string s("Hello World!");
    m_test_obj_.set_parameter("s", &s);
    EXPECT_TRUE(m_test_obj_.execute("reset"));
    i = 5;
    EXPECT_TRUE(m_test_obj_.get_parameter("i", &i));
    EXPECT_EQ(0, i);
    d = 5.0;
    EXPECT_TRUE(m_test_obj_.get_parameter("d", &d));
    EXPECT_EQ(0.0, d);
    s = "non-empty";
    EXPECT_TRUE(m_test_obj_.get_parameter("s", &s));
    EXPECT_EQ("", s);
    EXPECT_TRUE(m_test_obj_.execute("run"));
}

TEST_F(IAbstractTest, ExecuteWithParaAndRet) {
    int i = 10;
    m_test_obj_.set_parameter("i", &i);
    std::string s("Hello World!");
    m_test_obj_.set_parameter("s", &s);
    std::string out;
    int ip = 2;
    std::string sp = " And once more: Hello World!";
    void* param[] = {&ip, &sp};
    EXPECT_TRUE(m_test_obj_.execute("calcSth", param, &out));
    EXPECT_EQ("20;\"Hello World! And once more: Hello World!\"", out);
}
