#pragma once

#include <Eigen/Core>

#include <filesystem>
#include <unordered_map>
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
    virtual ~branch() = default;
    
    virtual std::unique_ptr<node> make_child(const std::string& key) const = 0;

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;

    std::unordered_map<std::string, std::unique_ptr<node>> children;
    node& operator[](const std::string& key);
};

template <typename T>
struct leaf : node
{
    leaf() = default;
    leaf(T v) : value(v) {}
    
    T value;
    operator T&() { return value; }

    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;
};

template <typename T>
bool operator==(const dotraiser::leaf<T>& lhs, const T& rhs) { return lhs.value == rhs; }

template <typename T>
bool operator!=(const dotraiser::leaf<T>& lhs, const T& rhs) { return !(lhs == rhs); }

struct camera : branch
{
    Eigen::Vector3f& get_position()    { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("position")); };
    Eigen::Vector3f& get_viewdir()     { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("viewdir")); };
    Eigen::Vector3f& get_updir()       { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("updir")); };
    float&           get_aspectratio() { return static_cast<leaf<float>&>          (operator[]("aspectratio")); };

    virtual std::unique_ptr<node> make_child(const std::string& key) const override;
};

struct ambient_light : branch
{
    Eigen::Vector3f& get_color() { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("color")); };

    virtual std::unique_ptr<node> make_child(const std::string& key) const override;
};

struct point_light : branch
{
    Eigen::Vector3f& get_position() { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("position")); };
    Eigen::Vector3f& get_color()    { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("color")); };

    virtual std::unique_ptr<node> make_child(const std::string& key) const override;
};

struct directional_light : branch
{
    Eigen::Vector3f& get_direction() { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("direction")); };
    Eigen::Vector3f& get_color()     { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("color")); };

    virtual std::unique_ptr<node> make_child(const std::string& key) const override;
};

struct material : branch
{
    Eigen::Vector3f& get_emissive()     { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("emissive")); };
    Eigen::Vector3f& get_ambient()      { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("ambient")); };
    Eigen::Vector3f& get_specular()     { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("specular")); };
    Eigen::Vector3f& get_reflective()   { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("reflective")); };
    Eigen::Vector3f& get_diffuse()      { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("diffuse")); };
    Eigen::Vector3f& get_transmissive() { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("transmissive")); };
    float&           get_shininess()    { return static_cast<leaf<float>&>          (operator[]("shininess")); };
    float&           get_index()        { return static_cast<leaf<float>&>          (operator[]("index")); };
    std::string&     get_name()         { return static_cast<leaf<std::string>&>    (operator[]("name")); };

    virtual std::unique_ptr<node> make_child(const std::string& key) const override;
};

struct object : branch
{
    virtual ~object() = default;

    material&        get_material()  { return static_cast<material&>             (operator[]("material")); };
    Eigen::Vector3f& get_translate() { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("translate")); };
    Eigen::Vector3f& get_scale()     { return static_cast<leaf<Eigen::Vector3f>&>(operator[]("scale")); };
    Eigen::Vector4f& get_rotate()    { return static_cast<leaf<Eigen::Vector4f>&>(operator[]("rotate")); };
    Eigen::Matrix4f& get_transform() { return static_cast<leaf<Eigen::Matrix4f>&>(operator[]("transform")); };

    virtual std::unique_ptr<node> make_child(const std::string& key) const override;
};

struct polymesh : object
{
    std::string get_objfile() { return static_cast<leaf<std::string>&>(operator[]("objfile")); };

    virtual std::unique_ptr<node> make_child(const std::string& key) const override;
};

struct trunk : node
{
    virtual ~trunk() = default;
    
    std::istream& parse(std::istream& is) override;
    std::ostream& print(std::ostream& os) const override;

    std::unique_ptr<camera> cam;
    std::vector<std::unique_ptr<ambient_light>> ambient_lights;
    std::vector<std::unique_ptr<point_light>> point_lights;
    std::vector<std::unique_ptr<directional_light>> directional_lights;
    std::vector<std::unique_ptr<object>> objects;
};

}

std::istream& operator>>(std::istream& is, dotraiser::node& n);
std::ostream& operator<<(std::ostream& os, const dotraiser::node& n);