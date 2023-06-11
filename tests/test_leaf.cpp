#include "dotraiser/dotraiser.hpp"

#include <gtest/gtest.h>

#include <sstream>

TEST(TestNode, Camera)
{
    std::stringstream ss(
        "{ position = (0,0,0); viewdir = (0,0,1); updir = (0,1,0); aspectratio = 1.0; }"
    );
    
    dotraiser::camera c;
    ss >> c;

    EXPECT_EQ(c.get_position(), Eigen::Vector3f( 0, 0, 0 ))  << "Camera position should be (0,0,0)";
    EXPECT_EQ(c.get_viewdir(),  Eigen::Vector3f( 0, 0, 1 ))  << "Camera viewdir should be (0,0,1)";
    EXPECT_EQ(c.get_updir(),    Eigen::Vector3f( 0, 1, 0 ))  << "Camera updir should be (0,1,0)";
    EXPECT_EQ(c.get_aspectratio(), 1.0)                      << "Camera aspectratio should be 1.0";
}

TEST(TestNode, AmbientLight)
{
    std::stringstream ss(
        "{ color = (1,1,1); }"
    );
    
    dotraiser::ambient_light al;
    ss >> al;

    EXPECT_EQ(al.get_color(), Eigen::Vector3f( 1, 1, 1 ))  << "Ambient light color should be (1,1,1)";
}

TEST(TestNode, PointLight)
{
    std::stringstream ss(
        "{ position = (0,0,0); color = (1,1,1); }"
    );
    
    dotraiser::point_light pl;
    ss >> pl;

    EXPECT_EQ(pl.get_position(), Eigen::Vector3f( 0, 0, 0 ))  << "Point light position should be (0,0,0)";
    EXPECT_EQ(pl.get_color(),    Eigen::Vector3f( 1, 1, 1 ))  << "Point light color should be (1,1,1)";
}

TEST(TestNode, directional_light)
{
    std::stringstream ss(
        "{ direction = (0,0,0); color = (1,1,1); }"
    );
    
    dotraiser::directional_light dl;
    ss >> dl;

    EXPECT_EQ(dl.get_direction(), Eigen::Vector3f( 0, 0, 0 ))  << "Directional light direction should be (0,0,0)";
    EXPECT_EQ(dl.get_color(),     Eigen::Vector3f( 1, 1, 1 ))  << "Directional light color should be (1,1,1)";
}

TEST(TestNode, Material)
{
    std::stringstream ss(
        "{ emissive = (1,1,1); ambient = (1,1,1); specular = (1,1,1); reflective = (1,0,1); diffuse = (1,1,1); transmissive = (0, 0, 1); shininess = 1.0; index = 0.5; name = \"gold\"; }"
    );

    dotraiser::material m;
    ss >> m;

    EXPECT_EQ(m.get_emissive(),     Eigen::Vector3f( 1, 1, 1 ))  << "Material emissive should be (1,1,1)";
    EXPECT_EQ(m.get_ambient(),      Eigen::Vector3f( 1, 1, 1 ))  << "Material ambient should be (1,1,1)";
    EXPECT_EQ(m.get_specular(),     Eigen::Vector3f( 1, 1, 1 ))  << "Material specular should be (1,1,1)";
    EXPECT_EQ(m.get_reflective(),   Eigen::Vector3f( 1, 0, 1 ))  << "Material reflective should be (1,0,1)";
    EXPECT_EQ(m.get_diffuse(),      Eigen::Vector3f( 1, 1, 1 ))  << "Material diffuse should be (1,1,1)";
    EXPECT_EQ(m.get_transmissive(), Eigen::Vector3f( 0, 0, 1 ))  << "Material transmissive should be (0,0,1)";
    EXPECT_EQ(m.get_shininess(),    1.0)                         << "Material shininess should be 1.0";
    EXPECT_EQ(m.get_index(),        0.5)                         << "Material index should be 0.5";
    EXPECT_EQ(m.get_name(),         "gold")                      << "Material name should be \"gold\"";
}

TEST(TestNode, Polymesh)
{
    std::stringstream ss(
        " { material = { emissive = (1,1,1); diffuse = (1,1,1); transmissive = (0, 0, 1); shininess = 1.0; }; objfile = \"/path/to/object.obj\"; }"
    );

    dotraiser::polymesh p;
    ss >> p;

    EXPECT_EQ(p.get_material().get_emissive(),     Eigen::Vector3f( 1, 1, 1 ))  << "Material emissive should be (1,1,1)";
    EXPECT_EQ(p.get_material().get_diffuse(),      Eigen::Vector3f( 1, 1, 1 ))  << "Material diffuse should be (1,1,1)";
    EXPECT_EQ(p.get_material().get_transmissive(), Eigen::Vector3f( 0, 0, 1 ))  << "Material transmissive should be (0,0,1)";
    EXPECT_EQ(p.get_material().get_shininess(),    1.0)                         << "Material shininess should be 1.0";
    EXPECT_EQ(p.get_objfile(),                     "/path/to/object.obj")    << "Path should be \"/path/to/object.obj\"";
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}