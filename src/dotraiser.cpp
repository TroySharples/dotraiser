#include "dotraiser.hpp"

#include <unordered_map>
#include <stdexcept>

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
    char oc;
    is >> oc;
    if (oc != '{')
        throw std::runtime_error("Camera format must begin with '{'");

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
        
        char sc;
        is >> sc;
        if (sc != ';')
            throw std::runtime_error("Key must end with ';'");
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

}

std::istream& operator>>(std::istream& is, dotraiser::node& n) { return n.parse(is); }
std::ostream& operator<<(std::ostream& os, const dotraiser::node& n) { return n.print(os); }

std::istream& operator>>(std::istream& is, Eigen::Vector3f& v)
{
    char op, c0, c1, cp;
    is >> op >> v.x() >> c0 >> v.y() >> c1 >> v.z() >> cp;
    if (op != '(' && c0 != ',' && c1 != ',' && cp != ')')
        throw std::runtime_error("Invalid vector format");
    return is;
}
