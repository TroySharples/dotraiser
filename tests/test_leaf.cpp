#include "dotraiser/dotraiser.hpp"

#include <gtest/gtest.h>

#include <sstream>

TEST(TestLeaf, Camera)
{
    std::stringstream ss(
        "{ position (0,0,0); viewdir (0,0,1); updir (0,1,0); aspectratio 1.0; }"
    );
    
    dotraiser::camera c;
    ss >> c;

    EXPECT_EQ(c.position, Eigen::Vector3f( 0, 0, 0 ))  << "Camera position should be (0,0,0)";
    EXPECT_EQ(c.viewdir,  Eigen::Vector3f( 0, 0, 1 ))  << "Camera viewdir should be (0,0,1)";
    EXPECT_EQ(c.updir,    Eigen::Vector3f( 0, 1, 0 ))  << "Camera updir should be (0,1,0)";
    EXPECT_EQ(c.aspectratio, 1.0) << "Camera aspectratio should be 1.0";
}

TEST(TestLeaf, CameraMissingSemicolon)
{
    std::stringstream ss(
        "{ position (0,0,0); viewdir (0,0,1) updir (0,1,0); aspectratio 1.0; }"
    );
    
    dotraiser::camera c;

    EXPECT_THROW(ss >> c, std::runtime_error) << "Camera should throw an error when missing a semicolon";
}

TEST(TestLeaf, CameraBadKey)
{
    std::stringstream ss(
        "{ position (0,0,0); oingo (0,0,1) updir (0,1,0); aspectratio 1.0; }"
    );
    
    dotraiser::camera c;

    EXPECT_THROW(ss >> c, std::runtime_error) << "Camera should throw an error when parsing a bad key";
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}