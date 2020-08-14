#include "toolib/filesys/path.h"
#include "gtest/gtest.h"

using too::file::Path;


class PathTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(PathTest, EmptyConstruction)
{
    Path p;
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

TEST_F(PathTest, Statics)
{
#if TOO_OS_WINDOWS
    EXPECT_EQ("\\", Path::getSeparatorNative());
#else
    EXPECT_EQ("/", Path::getSeparatorNative());
#endif
    Path p("/");
    std::string s(p.cleanupPlatformIndep());
    EXPECT_EQ(s, Path::getSeparatorPlatformIndep());
    s = p.cleanupNative();
    EXPECT_EQ(s, Path::getSeparatorNative());
}

TEST_F(PathTest, Constructions)
{
    std::string pstr("test");
    Path p(pstr);
    p.ensureTrailingSeparator();
    EXPECT_EQ("test", pstr);
    Path pr(pstr, true);
    pr.ensureTrailingSeparator();
    EXPECT_EQ("test/", pstr);
    Path pconst("test");
    pconst.ensureTrailingSeparator();
    std::string str(pconst);
    EXPECT_EQ("test/", str);
    Path cpy(pconst);
    str = cpy;
    EXPECT_EQ("test/", str);
    Path cpy2(pr);
    str = cpy2;
    EXPECT_EQ("test/", str);
    cpy2 = Path("a");
    str = cpy2;
    EXPECT_EQ("a", str);
    EXPECT_EQ("test/", pstr);
}

TEST_F(PathTest, Swapping)
{
    Path p1("p1", Path::EForm::PLATFORMINDEPENDENT, Path::EType::IS_FOLDER);
    std::string s2("p2");
    Path p2(s2, true, Path::EForm::NATIVE, Path::EType::IS_FILE);
    p1.swap(p2);
    std::string str(p2);
    EXPECT_EQ("p1", str);
    str = p1;
    EXPECT_EQ("p2", str);
}

TEST_F(PathTest, ensureTrailingSeparator)
{
    Path p("");
    std::string s(p.ensureTrailingSeparator());
    EXPECT_TRUE(s.empty());
    p = Path("a");
    s = p.ensureTrailingSeparator();
    EXPECT_EQ("a/", s);
    Path p2("a", Path::EForm::NATIVE);
    s = p2.ensureTrailingSeparator();
#if TOO_OS_WINDOWS
    EXPECT_EQ("a\\", s);
#else
    EXPECT_EQ("a/", s);
#endif
}

TEST_F(PathTest, AppendTo)
{
    Path p1("a/b");
    Path p2("c\\d/e");
    p1 += p2;
    std::string s(p1);
    EXPECT_EQ("a/b/c/d/e/", s);
}

TEST_F(PathTest, getFolderPath)
{
    Path p1("a/b");
    EXPECT_EQ("a/", p1.getFolderPath());
#if TOO_OS_WINDOWS
    Path p2("a\\b", Path::EForm::NATIVE);
    EXPECT_EQ("a\\", p2.getFolderPath());
#else
    Path p2("a/b", Path::EForm::NATIVE);
    EXPECT_EQ("a/", p2.getFolderPath());
#endif
}

TEST_F(PathTest, getFileName)
{
    Path p1("a/b");
    EXPECT_EQ("b", p1.getFileName());
    Path p2("a/b.exe");
    EXPECT_EQ("b.exe", p2.getFileName());
}

TEST_F(PathTest, getExtension)
{
    Path p1("a/b.ext");
    EXPECT_EQ("ext", p1.getExtension());
}

TEST_F(PathTest, isAbsolute)
{
    Path p1("a/b");
    EXPECT_FALSE(p1.isAbsolute());
#if TOO_OS_WINDOWS
    Path p2("c:\\a\\b");
    EXPECT_TRUE(p2.isAbsolute());
#else
    Path p2("/a/b");
    EXPECT_TRUE(p2.isAbsolute());
#endif
}

TEST_F(PathTest, cleanupX)
{
    Path p1("a/b/c");
    std::string s(p1.cleanupPlatformIndep());
    EXPECT_EQ("a/b/c", s);
    s = p1.cleanupNative();
#if TOO_OS_WINDOWS
    EXPECT_EQ("a\\b\\c", s);
#else
    EXPECT_EQ("a/b/c", s);
#endif
}

TEST_F(PathTest, getSeparatorUsedHere)
{
    Path p1("a/b");
    EXPECT_EQ("/", p1.getSeparatorUsedHere());
#if TOO_OS_WINDOWS
    Path p2("a\\b", Path::EForm::UNKNOWN);
    EXPECT_EQ("\\", p2.getSeparatorUsedHere());
#else
    Path p2("a/b", Path::EForm::UNKNOWN);
    EXPECT_EQ("/", p2.getSeparatorUsedHere());
#endif
#if TOO_OS_WINDOWS
    Path p3("a\\b", Path::EForm::NATIVE);
    EXPECT_EQ("\\", p3.getSeparatorUsedHere());
#else
    Path p3("a/b", Path::EForm::NATIVE);
    EXPECT_EQ("/", p3.getSeparatorUsedHere());
#endif
}
