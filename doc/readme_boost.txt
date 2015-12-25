SDK: boost
Version: 1.56.0
Folder link to archived originals:
  /projects/_dev_utils_archive/sdks, libs/C++/boost_1_56_0.zip
Folder link to installation for actual usage (from here):
  /projects/externlibs/boost_1_56_0

How to build pieces of boost:

Example which succeeded in building date_time with mingw:
-add mingw compiler to path variable
-run "bootstrap.bat mingw" from boost root
-run "b2 toolset=gcc --build-type=complete stage --with-date_time"
-libs are to be found in stage subdir