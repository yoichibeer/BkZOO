#include <gtest/gtest.h>

#include "debug/detect_memory_leak.h"

int main(int argc, char* argv[])
{
#ifdef _DEBUG
    ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif //_DEBUG

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
