cd ..

rem "C:\Program Files\Cppcheck\cppcheck.exe" -iUnitTest -iDebug -iBkZOOTest -iRelease -iBkZOOSetup -ishare -iuncrustify -ivendor  -IBkZOOLib -IBkZOOLib/bkzoo  -IBkZOO -IBkZOO/bkzoo -Ivendor/gtest-1.7.0/include -D_MSC_VER=1900 -D_WIN32=1 -DWIN32=1 -D_WINDOWS=1 -D_USRDLL=1 -D_CRT_SECURE_NO_WARNINGS=1 -DBKZOO_EXPORTS=1 -DNDEBUG=1 --enable=all --suppress=missingIncludeSystem --inline-suppr --xml-version=2 --xml .  2> cppcheck_release_product.xml

rem "C:\Program Files\Cppcheck\cppcheck.exe" -iDebug -iRelease -iBkZOOSetup -ishare -iuncrustify -ivendor -IBkZOOLib -IBkZOOLib/bkzoo -IBkZOO -IBkZOO/bkzoo -IBkZOOTest -IUnitTest -D_MSC_VER=1900 -D_WIN32=1 -DWIN32=1 -D_WINDOWS=1 -D_USRDLL=1 -D_CRT_SECURE_NO_WARNINGS=1 -DBKZOO_EXPORTS=1 -D_DEBUG=1 --enable=all --suppressions-list=suppressions.txt --inline-suppr --xml-version=2 --xml .  2> cppcheck_debug_all.xml

rem "C:\Program Files\Cppcheck\cppcheck.exe" -iDebug -iRelease -iBkZOOSetup -ishare -iuncrustify -ivendor -IBkZOOLib -IBkZOOLib/bkzoo -IBkZOO -IBkZOO/bkzoo -IBkZOOTest -IUnitTest -D_MSC_VER=1900 -D_WIN32=1 -DWIN32=1 -D_WINDOWS=1 -D_USRDLL=1 -D_CRT_SECURE_NO_WARNINGS=1 -DBKZOO_EXPORTS=1 -DNDEBUG=1 --enable=all --suppressions-list=suppressions.txt --inline-suppr --xml-version=2 --xml .  2> cppcheck_release_all.xml

"C:\Program Files\Cppcheck\cppcheck.exe" -iDebug -iRelease -iBkZOOSetup -ishare -iuncrustify -ivendor -IBkZOOLib -IBkZOOLib/bkzoo -IBkZOO -IBkZOO/bkzoo -IBkZOOTest -IUnitTest --enable=all --suppressions-list=cppcheck/cppcheck_suppressions.txt --inline-suppr --xml-version=2 --xml .  2> cppcheck/cppcheck_result.xml


pause
