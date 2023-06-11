#include "dotraiser.hpp"

#include <unordered_map>
#include <stdexcept>

// Helper parser functions
static std::istream& parse_charactor(std::istream& is, char c)
{
    char v;
    is >> v;
    if (v != c)
        throw std::runtime_error("Missing charector: " + std::string(1, c));
    return is;
}

static std::istream& parse_escaped_string(std::istream& is, std::string& str)
{
    std::getline(is, str, '"');
    std::getline(is, str, '"');
    return is;
}

namespace dotraiser
{

// Map of node constructors
using constructor_t = std::unique_ptr<node>(*)();
using factory_t     = std::unordered_map<std::string, constructor_t>;
static factory_t make_factory()
{
    factory_t ret;
    ret[camera::LABEL] = []() { return static_cast<std::unique_ptr<node>>(std::make_unique<camera>()); };
    return ret;
}
const static factory_t factory = make_factory();

std::istream& camera::parse(std::istream& is)
{
    parse_charactor(is, '{');

    std::string key;
    while (is >> key)
    {
        if (key == "position")
            is >> position;
        else if (key == "viewdir")
            is >> viewdir;
        else if (key == "updir")
            is >> updir;
        else if (key == "aspectratio")
            is >> aspectratio;
        else if (key == "}")
            break;
        else
            throw std::runtime_error("Invalid camera key: " + key);
        
        parse_charactor(is, ';');
    }
    return is;
}

std::ostream& camera::print(std::ostream& os) const
{
    return os << "{ "
              << "position "    << position    << "; "
              << "viewdir "     << viewdir     << "; "
              << "updir "       << updir       << "; "
              << "aspectratio " << aspectratio << "; "
              << "}";
}

std::istream& ambient_light::parse(std::istream& is)
{
    parse_charactor(is, '{');

    std::string key;
    while (is >> key)
    {
        if (key == "color")
            is >> color;
        else if (key == "}")
            break;
        else
            throw std::runtime_error("Invalid ambient_light key: " + key);
        
        parse_charactor(is, ';');
    }
    return is;

}

std::ostream& ambient_light::print(std::ostream& os) const
{
    return os << "{ "
              << "color " << color << "; "
              << "}";
}

std::istream& point_light::parse(std::istream& is)
{
    parse_charactor(is, '{');

    std::string key;
    while (is >> key)
    {
        if (key == "position")
            is >> position;
        else if (key == "color")
            is >> color;
        else if (key == "}")
            break;
        else
            throw std::runtime_error("Invalid point_light key: " + key);
        
        parse_charactor(is, ';');
    }
    return is;
}

std::ostream& point_light::print(std::ostream& os) const
{
    return os << "{ "
              << "position " << position << "; "
              << "color "    << color    << "; "
              << "}";
}

std::istream& directional_light::parse(std::istream& is)
{
    parse_charactor(is, '{');

    std::string key;
    while (is >> key)
    {
        if (key == "direction")
            is >> direction;
        else if (key == "color")
            is >> color;
        else if (key == "}")
            break;
        else
            throw std::runtime_error("Invalid directional_light key: " + key);
        
        parse_charactor(is, ';');
    }
    return is;
}

std::ostream& directional_light::print(std::ostream& os) const
{
    return os << "{ "
              << "direction " << direction << "; "
              << "color "     << color     << "; "
              << "}";
}

std::istream& material::parse(std::istream& is)
{
    parse_charactor(is, '{');

    std::string key;
    while (is >> key)
    {
        if (key == "emissive")
            is >> emissive;
        else if (key == "ambient")
            is >> ambient;
        else if (key == "specular")
            is >> specular;
        else if (key == "reflective")
            is >> reflective;
        else if (key == "diffuse")
            is >> diffuse;
        else if (key == "transmissive")
            is >> transmissive;
        else if (key == "shininess")
            is >> shininess;
        else if (key == "index")
            is >> index;
        else if (key == "name")
            parse_escaped_string(is, name);
        else if (key == "}")
            break;
        else
            throw std::runtime_error("Invalid material key: " + key);
        
        parse_charactor(is, ';');
    }
    return is;
}

std::ostream& material::print(std::ostream& os) const
{
    return os << "{ "
              << "emissive "     << emissive     << "; "
              << "ambient "      << ambient      << "; "
              << "specular "     << specular     << "; "
              << "reflective "   << reflective   << "; "
              << "diffuse "      << diffuse      << "; "
              << "transmissive " << transmissive << "; "
              << "shininess "    << shininess    << "; "
              << "index "        << index        << "; "
              << "name "         << name         << "; "
              << "}";
}

}

std::istream& operator>>(std::istream& is, dotraiser::node& n) { return n.parse(is); }
std::ostream& operator<<(std::ostream& os, const dotraiser::node& n) { return n.print(os); }

std::istream& operator>>(std::istream& is, Eigen::Vector3f& v)
{
    parse_charactor(is, '(');
    is >> v.x();
    parse_charactor(is, ',');
    is >> v.y();
    parse_charactor(is, ',');
    is >> v.z();
    parse_charactor(is, ')');
    return is;
}
