#include "gtest/gtest.h"
#include "Toolib/filesys/path.h"

using too::file::CPath;

class CPathTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

};

TEST_F(CPathTest, EmptyConstruction)
{
    CPath p;
    too::string s(p);
    EXPECT_TRUE(s.empty());
    p.ensureTrailingSeparator(false);
    s = p;
    EXPECT_EQ(_TOOSTR(""), s);
    p.ensureTrailingSeparator(true);
    s = p;
    EXPECT_EQ(_TOOSTR(""), s);
    p.cleanupNative();
    s = p;
    EXPECT_EQ(_TOOSTR(""), s);
    p.cleanupPlatformIndep();
    s = p;
    EXPECT_EQ(_TOOSTR(""), s);
    EXPECT_EQ(_TOOSTR(""), p.getFolderPath());
    EXPECT_EQ(_TOOSTR(""), p.getFileName());
    EXPECT_EQ(_TOOSTR(""), p.getExtension());
    EXPECT_FALSE(p.isAbsolute());
    EXPECT_TRUE(p.isEmpty());
}

TEST_F(CPathTest, Statics)
{
#if TOO_OS_WINDOWS
    EXPECT_EQ(_TOOSTR("\\"), CPath::getSeparatorNative());
#else
    EXPECT_EQ(_TOOSTR("/"), CPath::getSeparatorNative());
#endif
    CPath p(_TOOSTR("/"));
    too::string s(p.cleanupPlatformIndep());
    EXPECT_EQ(s, CPath::getSeparatorPlatformIndep());
    s = p.cleanupNative();
    EXPECT_EQ(s, CPath::getSeparatorNative());
}

TEST_F(CPathTest, Constructions)
{
    too::string pstr(_TOOSTR("test"));
    CPath p(pstr);
    p.ensureTrailingSeparator();
    EXPECT_EQ(_TOOSTR("test"), pstr);
    CPath pr(pstr, true);
    pr.ensureTrailingSeparator();
    EXPECT_EQ(_TOOSTR("test/"), pstr);
    CPath pconst(_TOOSTR("test"));
    pconst.ensureTrailingSeparator();
    too::string str(pconst);
    EXPECT_EQ(_TOOSTR("test/"), str);
    CPath cpy(pconst);
    str = cpy;
    EXPECT_EQ(_TOOSTR("test/"), str);
    CPath cpy2(pr);
    str = cpy2;
    EXPECT_EQ(_TOOSTR("test/"), str);
    cpy2 = CPath(_TOOSTR("a"));
    str = cpy2;
    EXPECT_EQ(_TOOSTR("a"), str);
    EXPECT_EQ(_TOOSTR("test/"), pstr);
}

TEST_F(CPathTest, Swapping)
{
    CPath p1(_TOOSTR("p1"), CPath::EForm::PLATFORMINDEPENDENT, CPath::EType::IS_FOLDER);
    too::string s2(_TOOSTR("p2"));
    CPath p2(s2, true, CPath::EForm::NATIVE, CPath::EType::IS_FILE);
    p1.swap(p2);
    too::string str(p2);
    EXPECT_EQ(_TOOSTR("p1"), str);
    str = p1;
    EXPECT_EQ(_TOOSTR("p2"), str);
}

TEST_F(CPathTest, ensureTrailingSeparator)
{
    CPath p(_TOOSTR(""));
    too::string s(p.ensureTrailingSeparator());
    EXPECT_TRUE(s.empty());
    p = CPath(_TOOSTR("a"));
    s = p.ensureTrailingSeparator();
    EXPECT_EQ(_TOOSTR("a/"), s);
    CPath p2(_TOOSTR("a"), CPath::EForm::NATIVE);
    s = p2.ensureTrailingSeparator();
#if TOO_OS_WINDOWS
    EXPECT_EQ(_TOOSTR("a\\"), s);
#else
    EXPECT_EQ(_TOOSTR("a/"), s);
#endif
}

TEST_F(CPathTest, AppendTo)
{
    CPath p1(_TOOSTR("a/b"));
    CPath p2(_TOOSTR("c\\d/e"));
    p1+= p2;
    too::string s(p1);
    EXPECT_EQ(_TOOSTR("a/b/c/d/e/"), s);
}

TEST_F(CPathTest, getFolderPath)
{
    CPath p1(_TOOSTR("a/b"));
    EXPECT_EQ(_TOOSTR("a/"), p1.getFolderPath());
    CPath p2(_TOOSTR("a\\b"), CPath::EForm::NATIVE);
    EXPECT_EQ(_TOOSTR("a\\"), p2.getFolderPath());
}

TEST_F(CPathTest, getFileName)
{
    CPath p1(_TOOSTR("a/b"));
    EXPECT_EQ(_TOOSTR("b"), p1.getFileName());
}

TEST_F(CPathTest, getExtension)
{
    CPath p1(_TOOSTR("a/b.ext"));
    EXPECT_EQ(_TOOSTR("ext"), p1.getExtension());
}

TEST_F(CPathTest, isAbsolute)
{
    CPath p1(_TOOSTR("a/b"));
    EXPECT_FALSE(p1.isAbsolute());
#if TOO_OS_WINDOWS
    CPath p2(_TOOSTR("c:\\a\\b"));
    EXPECT_TRUE(p2.isAbsolute());
#else
    CPath p2(_TOOSTR("/a/b"));
    EXPECT_TRUE(p2.isAbsolute());
#endif
}

TEST_F(CPathTest, cleanupX)
{
    CPath p1(_TOOSTR("a/b/c"));
    too::string s(p1.cleanupPlatformIndep());
    EXPECT_EQ(_TOOSTR("a/b/c"), s);
    s = p1.cleanupNative();
#if TOO_OS_WINDOWS
    EXPECT_EQ(_TOOSTR("a\\b\\c"), s);
#else
    EXPECT_EQ(_TOOSTR("a/b/c"), s);
#endif
}

TEST_F(CPathTest, getSeparatorUsedHere)
{
    CPath p1(_TOOSTR("a/b"));
    EXPECT_EQ(_TOOSTR("/"), p1.getSeparatorUsedHere());
    CPath p2(_TOOSTR("a\\b"), CPath::EForm::UNKNOWN);
    EXPECT_EQ(_TOOSTR("\\"), p2.getSeparatorUsedHere());
    CPath p3(_TOOSTR("a\\b"), CPath::EForm::NATIVE);
    EXPECT_EQ(_TOOSTR("\\"), p3.getSeparatorUsedHere());
}
