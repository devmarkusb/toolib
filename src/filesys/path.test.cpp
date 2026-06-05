#include "mb/toolib/filesys/path.hpp"
#include "gtest/gtest.h"

using too::file::Path;

class PathTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(PathTest, EmptyConstruction) {
    Path p;
    std::string s(p);
    EXPECT_TRUE(s.empty());
    p.ensure_trailing_separator(false);
    s = p;
    EXPECT_EQ("", s);
    p.ensure_trailing_separator(true);
    s = p;
    EXPECT_EQ("", s);
    p.cleanup_native();
    s = p;
    EXPECT_EQ("", s);
    p.cleanup_platform_indep();
    s = p;
    EXPECT_EQ("", s);
    EXPECT_EQ("", p.get_folder_path());
    EXPECT_EQ("", p.get_file_name());
    EXPECT_EQ("", p.get_extension());
    EXPECT_FALSE(p.is_absolute());
    EXPECT_TRUE(p.is_empty());
}

TEST_F(PathTest, StaticMethods) {
#if UL_OS_WINDOWS
    EXPECT_EQ("\\", Path::get_separator_native());
#else
    EXPECT_EQ("/", Path::get_separator_native());
#endif
    Path p("/");
    std::string s(p.cleanup_platform_indep());
    EXPECT_EQ(s, Path::get_separator_platform_indep());
    s = p.cleanup_native();
    EXPECT_EQ(s, Path::get_separator_native());
}

TEST_F(PathTest, Constructions) {
    std::string pstr("test");
    Path p(pstr);
    p.ensure_trailing_separator();
    EXPECT_EQ("test", pstr);
    Path pr(pstr, true);
    pr.ensure_trailing_separator();
    EXPECT_EQ("test/", pstr);
    Path pconst("test");
    pconst.ensure_trailing_separator();
    std::string str(pconst);
    EXPECT_EQ("test/", str);
    const Path cpy(pconst);
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

TEST_F(PathTest, Swapping) {
    Path p1("p1", Path::EForm::platformindependent, Path::EType::is_folder);
    std::string s2("p2");
    Path p2(s2, true, Path::EForm::native, Path::EType::is_file);
    p1.swap(p2);
    std::string str(p2);
    EXPECT_EQ("p1", str);
    str = p1;
    EXPECT_EQ("p2", str);
}

TEST_F(PathTest, ensureTrailingSeparator) {
    Path p("");
    std::string s(p.ensure_trailing_separator());
    EXPECT_TRUE(s.empty());
    p = Path("a");
    s = p.ensure_trailing_separator();
    EXPECT_EQ("a/", s);
    Path p2("a", Path::EForm::native);
    s = p2.ensure_trailing_separator();
#if UL_OS_WINDOWS
    EXPECT_EQ("a\\", s);
#else
    EXPECT_EQ("a/", s);
#endif
}

TEST_F(PathTest, AppendTo) {
    Path p1("a/b");
    const Path p2("c\\d/e");
    p1 += p2;
    const std::string s(p1);
    EXPECT_EQ("a/b/c/d/e/", s);
}

TEST_F(PathTest, getFolderPath) {
    const Path p1("a/b");
    EXPECT_EQ("a/", p1.get_folder_path());
#if UL_OS_WINDOWS
    Path p2("a\\b", Path::EForm::native);
    EXPECT_EQ("a\\", p2.get_folder_path());
#else
    const Path p2("a/b", Path::EForm::native);
    EXPECT_EQ("a/", p2.get_folder_path());
#endif
}

TEST_F(PathTest, getFileName) {
    const Path p1("a/b");
    EXPECT_EQ("b", p1.get_file_name());
    const Path p2("a/b.exe");
    EXPECT_EQ("b.exe", p2.get_file_name());
}

TEST_F(PathTest, get_extension) {
    const Path p1("a/b.ext");
    EXPECT_EQ("ext", p1.get_extension());
}

TEST_F(PathTest, isAbsolute) {
    const Path p1("a/b");
    EXPECT_FALSE(p1.is_absolute());
#if UL_OS_WINDOWS
    Path p2("c:\\a\\b");
    EXPECT_TRUE(p2.is_absolute());
#else
    const Path p2("/a/b");
    EXPECT_TRUE(p2.is_absolute());
#endif
}

TEST_F(PathTest, cleanupX) {
    Path p1("a/b/c");
    std::string s(p1.cleanup_platform_indep());
    EXPECT_EQ("a/b/c", s);
    s = p1.cleanup_native();
#if UL_OS_WINDOWS
    EXPECT_EQ("a\\b\\c", s);
#else
    EXPECT_EQ("a/b/c", s);
#endif
}

TEST_F(PathTest, getSeparatorUsedHere) {
    const Path p1("a/b");
    EXPECT_EQ("/", p1.get_separator_used_here());
#if UL_OS_WINDOWS
    Path p2("a\\b", Path::EForm::unknown);
    EXPECT_EQ("\\", p2.get_separator_used_here());
#else
    const Path p2("a/b", Path::EForm::unknown);
    EXPECT_EQ("/", p2.get_separator_used_here());
#endif
#if UL_OS_WINDOWS
    Path p3("a\\b", Path::EForm::native);
    EXPECT_EQ("\\", p3.get_separator_used_here());
#else
    const Path p3("a/b", Path::EForm::native);
    EXPECT_EQ("/", p3.get_separator_used_here());
#endif
}
