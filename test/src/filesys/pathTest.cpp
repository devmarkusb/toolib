#include "gtest/gtest.h"
#include "Toolib/filesys/path.h"

using too::file::CPath;

class CPathTest : public ::testing::Test
{
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(CPathTest, EmptyConstruction)
{
    CPath p;
    std::string s(p);
    EXPECT_TRUE(s.empty());
    p.ensureTrailingSeparator(false);
    s = p;
    EXPECT_EQ("", s);
    p.ensureTrailingSeparator(true);
    s = p;
    EXPECT_EQ("", s);
    p.cleanupNative();
    s = p;
    EXPECT_EQ("", s);
    p.cleanupPlatformIndep();
    s = p;
    EXPECT_EQ("", s);
    EXPECT_EQ("", p.getFolderPath());
    EXPECT_EQ("", p.getFileName());
    EXPECT_EQ("", p.getExtension());
    EXPECT_FALSE(p.isAbsolute());
    EXPECT_TRUE(p.isEmpty());
}

TEST_F(CPathTest, Statics)
{
#if TOO_OS_WINDOWS
    EXPECT_EQ("\\", CPath::getSeparatorNative());
#else
    EXPECT_EQ("/"), CPath::getSeparatorNative());
#endif
    CPath p("/");
    std::string s(p.cleanupPlatformIndep());
    EXPECT_EQ(s, CPath::getSeparatorPlatformIndep());
    s = p.cleanupNative();
    EXPECT_EQ(s, CPath::getSeparatorNative());
}

TEST_F(CPathTest, Constructions)
{
    std::string pstr("test");
    CPath p(pstr);
    p.ensureTrailingSeparator();
    EXPECT_EQ("test", pstr);
    CPath pr(pstr, true);
    pr.ensureTrailingSeparator();
    EXPECT_EQ("test/", pstr);
    CPath pconst("test");
    pconst.ensureTrailingSeparator();
    std::string str(pconst);
    EXPECT_EQ("test/", str);
    CPath cpy(pconst);
    str = cpy;
    EXPECT_EQ("test/", str);
    CPath cpy2(pr);
    str = cpy2;
    EXPECT_EQ("test/", str);
    cpy2 = CPath("a");
    str = cpy2;
    EXPECT_EQ("a", str);
    EXPECT_EQ("test/", pstr);
}

TEST_F(CPathTest, Swapping)
{
    CPath p1("p1", CPath::EForm::PLATFORMINDEPENDENT, CPath::EType::IS_FOLDER);
    std::string s2("p2");
    CPath p2(s2, true, CPath::EForm::NATIVE, CPath::EType::IS_FILE);
    p1.swap(p2);
    std::string str(p2);
    EXPECT_EQ("p1", str);
    str = p1;
    EXPECT_EQ("p2", str);
}

TEST_F(CPathTest, ensureTrailingSeparator)
{
    CPath p("");
    std::string s(p.ensureTrailingSeparator());
    EXPECT_TRUE(s.empty());
    p = CPath("a");
    s = p.ensureTrailingSeparator();
    EXPECT_EQ("a/", s);
    CPath p2("a", CPath::EForm::NATIVE);
    s = p2.ensureTrailingSeparator();
#if TOO_OS_WINDOWS
    EXPECT_EQ("a\\", s);
#else
    EXPECT_EQ("a/"), s);
#endif
}

TEST_F(CPathTest, AppendTo)
{
    CPath p1("a/b");
    CPath p2("c\\d/e");
    p1 += p2;
    std::string s(p1);
    EXPECT_EQ("a/b/c/d/e/", s);
}

TEST_F(CPathTest, getFolderPath)
{
    CPath p1("a/b");
    EXPECT_EQ("a/", p1.getFolderPath());
    CPath p2("a\\b", CPath::EForm::NATIVE);
    EXPECT_EQ("a\\", p2.getFolderPath());
}

TEST_F(CPathTest, getFileName)
{
    CPath p1("a/b");
    EXPECT_EQ("b", p1.getFileName());
}

TEST_F(CPathTest, getExtension)
{
    CPath p1("a/b.ext");
    EXPECT_EQ("ext", p1.getExtension());
}

TEST_F(CPathTest, isAbsolute)
{
    CPath p1("a/b");
    EXPECT_FALSE(p1.isAbsolute());
#if TOO_OS_WINDOWS
    CPath p2("c:\\a\\b");
    EXPECT_TRUE(p2.isAbsolute());
#else
    CPath p2("/a/b"));
    EXPECT_TRUE(p2.isAbsolute());
#endif
}

TEST_F(CPathTest, cleanupX)
{
    CPath p1("a/b/c");
    std::string s(p1.cleanupPlatformIndep());
    EXPECT_EQ("a/b/c", s);
    s = p1.cleanupNative();
#if TOO_OS_WINDOWS
    EXPECT_EQ("a\\b\\c", s);
#else
    EXPECT_EQ("a/b/c"), s);
#endif
}

TEST_F(CPathTest, getSeparatorUsedHere)
{
    CPath p1("a/b");
    EXPECT_EQ("/", p1.getSeparatorUsedHere());
    CPath p2("a\\b", CPath::EForm::UNKNOWN);
    EXPECT_EQ("\\", p2.getSeparatorUsedHere());
    CPath p3("a\\b", CPath::EForm::NATIVE);
    EXPECT_EQ("\\", p3.getSeparatorUsedHere());
}
