#include <someClass.hpp> 
#include <gtest/gtest.h>


TEST(TGroup, test1)
{
    SomeClass sc;
    ASSERT_TRUE(3 == sc.sum(3, 0));
    ASSERT_TRUE(1 == sc.sum(2, -1));
}

TEST(TGroup, test2)
{
    SomeClass sc;
    ASSERT_TRUE(3 == sc.sum(3, 0));
    ASSERT_TRUE(1 == sc.sum(2, -1));
}