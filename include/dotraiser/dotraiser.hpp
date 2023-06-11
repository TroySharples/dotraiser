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
    Eigen::Vector3f position;
    Eigen::Vector3f viewdir;
    Eigen::Vector3f updir;
    float aspectratio;

    static constexpr const char* LABEL = "camera";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct ambient_light : leaf
{
    Eigen::Vector3f color;

    static constexpr const char* LABEL = "ambient_light";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct point_light : leaf
{
    Eigen::Vector3f position;
    Eigen::Vector3f color;

    static constexpr const char* LABEL = "point_light";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct directional_light : leaf
{
    Eigen::Vector3f direction;
    Eigen::Vector3f color;

    static constexpr const char* LABEL = "direction_light";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct object 





struct polymesh : leaf
{
    std::filesystem::path objfile;

    static constexpr const char* LABEL = "polymesh";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

struct material : leaf
{
    Eigen::Vector3f emissive;
    Eigen::Vector3f ambient;
    Eigen::Vector3f specular;
    Eigen::Vector3f reflective;
    Eigen::Vector3f diffuse;
    Eigen::Vector3f transmissive;
    float shininess;
    float index;
    std::string name;

    static constexpr const char* LABEL = "material";
    const char* get_label() const override { return LABEL; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

}

std::istream& operator>>(std::istream& is, dotraiser::node& n);
std::istream& operator>>(std::istream& is, Eigen::Vector3f& v);

std::ostream& operator<<(std::ostream& os, const dotraiser::node& n);