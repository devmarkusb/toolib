REM Updating translation ts files from sources (cpp+qml)
REM Expects path to lupdate from Qt install dir as first and only param.
%1\lupdate source gui -ts lang_en.ts
%1\lupdate source gui -ts lang_de.ts
