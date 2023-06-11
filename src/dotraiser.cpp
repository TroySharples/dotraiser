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

    while (static_cast<char>((is >> std::ws).peek()) != '}')
    {
        std::string key;
        is >> key;
        parse_charactor(is, '=');

        auto child = make_child(key);
        child->parse(is);
        children[key] = std::move(child);

        parse_charactor(is, ';');
    }
    
    return parse_charactor(is, '}');
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
    return is;
}

template <>
std::ostream& leaf<Eigen::Vector4f>::print(std::ostream& os) const
{
    return os << "(" << value.x() << ", " << value.y() << ", " << value.z() << ", " << value.z() << ")";
}

template <>
std::istream& leaf<Eigen::Matrix4f>::parse(std::istream& is)
{
    parse_charactor(is, '(');
    parse_charactor(is, '(');
    is >> value(0, 0);
    parse_charactor(is, ',');
    is >> value(0, 1);
    parse_charactor(is, ',');
    is >> value(0, 2);
    parse_charactor(is, ',');
    is >> value(0, 3);
    parse_charactor(is, ')');
    parse_charactor(is, ',');
    parse_charactor(is, '(');
    is >> value(1, 0);
    parse_charactor(is, ',');
    is >> value(1, 1);
    parse_charactor(is, ',');
    is >> value(1, 2);
    parse_charactor(is, ',');
    is >> value(1, 3);
    parse_charactor(is, ')');
    parse_charactor(is, ',');
    parse_charactor(is, '(');
    is >> value(2, 0);
    parse_charactor(is, ',');
    is >> value(2, 1);
    parse_charactor(is, ',');
    is >> value(2, 2);
    parse_charactor(is, ',');
    is >> value(2, 3);
    parse_charactor(is, ')');
    parse_charactor(is, ',');
    parse_charactor(is, '(');
    is >> value(3, 0);
    parse_charactor(is, ',');
    is >> value(3, 1);
    parse_charactor(is, ',');
    is >> value(3, 2);
    parse_charactor(is, ',');
    is >> value(4, 3);
    parse_charactor(is, ')');
    parse_charactor(is, ')');
    return is;
}

template <>
std::ostream& leaf<Eigen::Matrix4f>::print(std::ostream& os) const
{
    return os << "((" << value(0, 0) << ", " << value(0, 1) << ", " << value(0, 2) << ", " << value(0, 3) << "), "
              <<  "(" << value(1, 0) << ", " << value(1, 1) << ", " << value(1, 2) << ", " << value(1, 3) << "), "
              <<  "(" << value(2, 0) << ", " << value(2, 1) << ", " << value(2, 2) << ", " << value(2, 3) << "), "
              <<  "(" << value(3, 0) << ", " << value(3, 1) << ", " << value(3, 2) << ", " << value(3, 3) << "))";
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

std::unique_ptr<node> object::make_child(const std::string& key) const
{
    if (key == "material")
        return std::make_unique<material>();
    if (key == "translate" || key == "scale")
        return std::make_unique<leaf<Eigen::Vector3f>>();
    if (key == "rotate")
        return std::make_unique<leaf<Eigen::Vector4f>>();
    if (key == "transform")
        return std::make_unique<leaf<Eigen::Matrix4f>>();
    throw std::runtime_error("Invalid material key: " + key);
}

std::unique_ptr<node> polymesh::make_child(const std::string& key) const
{
    if (key == "objfile")
        return std::make_unique<leaf<std::string>>();
    return object::make_child(key);
}

std::istream& trunk::parse(std::istream& is)
{
    parse_charactor(is, '{');

    while (static_cast<char>((is >> std::ws).peek()) != '}')
    {
        std::string key;
        is >> key;
        parse_charactor(is, '=');

        if (key == "camera")
        {
            // We can only have one camera
            if (cam != nullptr)
                throw std::runtime_error("Multiple cameras defined");
            cam = std::make_unique<camera>();
            cam->parse(is);
        }
        else if (key == "ambient_light")
        {
            auto amb = std::make_unique<ambient_light>();
            amb->parse(is);
            ambient_lights.push_back(std::move(amb));
        }
        else if (key == "point_light")
        {
            auto pl = std::make_unique<point_light>();
            pl->parse(is);
            point_lights.push_back(std::move(pl));
        }
        else if (key == "directional_light")
        {
            auto dl = std::make_unique<directional_light>();
            dl->parse(is);
            directional_lights.push_back(std::move(dl));
        }
        else if (key == "polymesh")
        {
            auto t = std::make_unique<polymesh>();
            t->parse(is);
            objects.push_back(std::move(t));
        }
        else
        {
            throw std::runtime_error("Invalid key: " + key);
        }

        parse_charactor(is, ';');
    }

    return parse_charactor(is, '}');
}

std::ostream& trunk::print(std::ostream& os) const
{
    os << "camera = " << *cam << ";\n";
    for (const auto& amb : ambient_lights)
        os << "ambient_light = " << *amb << ";\n";
    for (const auto& pl : point_lights)
        os << "point_light = " << *pl << ";\n";
    for (const auto& dl : directional_lights)
        os << "directional_light = " << *dl << ";\n";
    for (const auto& obj : objects)
        os << "polymesh = " << *obj << ";\n";
    return os;
}

}

std::istream& operator>>(std::istream& is, dotraiser::node& n) { return n.parse(is); }
std::ostream& operator<<(std::ostream& os, const dotraiser::node& n) { return n.print(os); }
