#include "dotraiser.hpp"

#include <stdexcept>

// Helper parser functions
static std::istream& parse_charactor(std::istream& is, char c)
{
    char v = 0;
    is >> v;
    if (v != c)
        throw std::runtime_error("Missing charector: " + std::string(1, c));
    return is;
}

namespace dotraiser
{

std::istream& branch::parse(std::istream& is)
{
    parse_charactor(is, '{');

    std::string key;
    while (is >> key)
    {
        if (key == "}")
            break;
        parse_charactor(is, '=');

        auto child = make_child(key);
        child->parse(is);
        children[key] = std::move(child);

        parse_charactor(is, ';');
    }
    return is;
}

std::ostream& branch::print(std::ostream& os) const
{
    os << "{ ";
    for (const auto& [key, child] : children)
        os << key << " " << *child << "; ";
    return os << "}";
}

node& branch::operator[](const std::string& key)
{
    if (children.find(key) == children.end())
        throw std::runtime_error("Invalid key: " + key);
    return *children[key];
}

template <>
std::istream& leaf<std::string>::parse(std::istream& is)
{
    std::getline(is, value, '"');
    std::getline(is, value, '"');
    return is;
}

template <>
std::ostream& leaf<std::string>::print(std::ostream& os) const
{
    return os << '"' << value << '"';
}

template <>
std::istream& leaf<float>::parse(std::istream& is)
{
    return is >> value;
}

template <>
std::ostream& leaf<float>::print(std::ostream& os) const
{
    return os <<  value;
}

template <>
std::istream& leaf<Eigen::Vector3f>::parse(std::istream& is)
{
    parse_charactor(is, '(');
    is >> value.x();
    parse_charactor(is, ',');
    is >> value.y();
    parse_charactor(is, ',');
    is >> value.z();
    parse_charactor(is, ')');
    return is;
}

template <>
std::ostream& leaf<Eigen::Vector3f>::print(std::ostream& os) const
{
    return os << "(" << value.x() << ", " << value.y() << ", " << value.z() << ")";
}

template <>
std::istream& leaf<Eigen::Vector4f>::parse(std::istream& is)
{
    parse_charactor(is, '(');
    is >> value.x();
    parse_charactor(is, ',');
    is >> value.y();
    parse_charactor(is, ',');
    is >> value.z();
    parse_charactor(is, ',');
    is >> value.w();
    parse_charactor(is, ')');
    return is >> value.x() >> value.y() >> value.z();
}

template <>
std::ostream& leaf<Eigen::Vector4f>::print(std::ostream& os) const
{
    return os << "(" << value.x() << ", " << value.y() << ", " << value.z() << ", " << value.z() << ")";
}

std::unique_ptr<node> camera::make_child(const std::string& key) const
{
    if (key == "position" || key == "viewdir" || key == "updir")
        return std::make_unique<leaf<Eigen::Vector3f>>();
    if (key == "aspectratio")
        return std::make_unique<leaf<float>>();
    throw std::runtime_error("Invalid camera key: " + key);
}

std::unique_ptr<node> ambient_light::make_child(const std::string& key) const
{
    if (key == "color")
        return std::make_unique<leaf<Eigen::Vector3f>>();
    throw std::runtime_error("Invalid ambient_light key: " + key);
}

std::unique_ptr<node> point_light::make_child(const std::string& key) const
{
    if (key == "position" || key == "color")
        return std::make_unique<leaf<Eigen::Vector3f>>();
    throw std::runtime_error("Invalid point_light key: " + key);
}

std::unique_ptr<node> directional_light::make_child(const std::string& key) const
{
    if (key == "direction" || key == "color")
        return std::make_unique<leaf<Eigen::Vector3f>>();
    throw std::runtime_error("Invalid point_light key: " + key);
}

std::unique_ptr<node> material::make_child(const std::string& key) const
{
    if (key == "emissive" || key == "ambient" || key == "specular" || key == "reflective" || key == "diffuse" || key == "transmissive")
        return std::make_unique<leaf<Eigen::Vector3f>>();
    if (key == "shininess" || key == "index")
        return std::make_unique<leaf<float>>();
    if (key == "name")
        return std::make_unique<leaf<std::string>>();
    throw std::runtime_error("Invalid material key: " + key);
}

std::unique_ptr<node> polymesh::make_child(const std::string& key) const
{
    if (key == "objfile")
        return std::make_unique<leaf<std::string>>();
    throw std::runtime_error("Invalid material key: " + key);
}

}

std::istream& operator>>(std::istream& is, dotraiser::node& n) { return n.parse(is); }
std::ostream& operator<<(std::ostream& os, const dotraiser::node& n) { return n.print(os); }
