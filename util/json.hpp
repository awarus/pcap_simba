#ifndef json_t_UTIL
#define json_t_UTIL

#include <cstdint>
#include <string>

#include <sstream>
#include <unordered_map>

class json_t {
public:
    json_t() {}

    json_t& add_double(const std::string& key, double value)
    {
        data[key] = std::to_string(value);
        return *this;
    }


    json_t& add(const std::string& key, std::string value)
    {
        data[key] = value;
        return *this;
    }

    json_t& add(const std::string& key, uint64_t value)
    {
        data[key] = std::to_string(value);
        return *this;
    }

    json_t& add(const std::string& key, const json_t& value)
    {
        data[key] = value.to_string();
        return *this;
    }

    std::string to_string() const {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto& pair : data) {
            if (!first) {
                oss << ",";
            }
            oss << "\"" << pair.first << "\":";
            oss << "\"" << pair.second << "\"";
            first = false;
        }
        oss << "}";
        return oss.str();
    }

private:
    std::unordered_map<std::string, std::string> data;
};

#endif /* json_t_UTIL */