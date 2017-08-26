/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_GTEST_H
#define BKZ_GTEST_H

 //
 // for gooogle test 
 //
#define FORWARD_DECLARE_CLASS_GTEST(test_case_name, test_name)\
class test_case_name##_##test_name##_Test

#define FRIEND_GTEST(test_case_name, test_name)\
friend FORWARD_DECLARE_CLASS_GTEST(test_case_name, test_name)


#endif // BKZ_GTEST_H
