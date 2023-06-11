#pragma once

#include <Eigen/Core>

#include <filesystem>
#include <vector>
#include <memory>
#include <iostream>

namespace dotraiser
{

struct node
{
    virtual ~node() = default;
    
    virtual std::istream& parse(std::istream& is) = 0;
    virtual std::ostream& print(std::ostream& os) const = 0;
};

struct branch : node
{
    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;

    std::vector<std::unique_ptr<node>> children;
};

struct leaf : node
{
    virtual ~leaf() = default;

    virtual const char* get_label() const = 0;
};

struct camera : leaf
{
    Eigen::Vector3f position { 0, 0, 0 };
    Eigen::Vector3f viewdir  { 1, 0, 0 };
    Eigen::Vector3f updir    { 0, 1, 0 };
    float aspectratio        { 1 };

    static constexpr const char* LABEL = "camera";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct ambient_light : leaf
{
    Eigen::Vector3f color { 0, 0, 0 };

    static constexpr const char* LABEL = "ambient_light";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct point_light : leaf
{
    Eigen::Vector3f position { 0, 0, 0 };
    Eigen::Vector3f color    { 0, 0, 0 };

    static constexpr const char* LABEL = "point_light";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct directional_light : leaf
{
    Eigen::Vector3f direction { 1, 0, 0 };
    Eigen::Vector3f color     { 0, 0, 0 };

    static constexpr const char* LABEL = "direction_light";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct material : leaf
{
    Eigen::Vector3f emissive      { 0, 0, 0 };
    Eigen::Vector3f ambient       { 0, 0, 0 };
    Eigen::Vector3f specular      { 0, 0, 0 };
    Eigen::Vector3f reflective    { 0, 0, 0 };
    Eigen::Vector3f diffuse       { 0, 0, 0 };
    Eigen::Vector3f transmissive  { 0, 0, 0 };
    float shininess               { 0 };
    float index                   { 0 };
    std::string name              { "unspecified" };

    static constexpr const char* LABEL = "material";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct object : leaf
{
    Eigen::Vector3f translate { 0, 0, 0 };
    float scale               { 1 };
    Eigen::Matrix3f rotate    { Eigen::Matrix3f::Identity() };
    Eigen::Matrix4f transform { Eigen::Matrix4f::Identity() };

    material mat;

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct polymesh : object
{
    std::filesystem::path objfile;

    static constexpr const char* LABEL = "polymesh";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

}

std::istream& operator>>(std::istream& is, dotraiser::node& n);
std::istream& operator>>(std::istream& is, Eigen::Vector3f& v);

std::ostream& operator<<(std::ostream& os, const dotraiser::node& n);