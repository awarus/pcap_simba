#ifndef json_t_UTIL
#define json_t_UTIL

#include <cstdint>
#include <string>
#include <vector>

#include <sstream>
#include <unordered_map>

class json_t {
public:
    json_t() {}

    json_t& add_double(const std::string& key, double value)
    {
        data[key].push_back(std::to_string(value));
        return *this;
    }

    json_t& add(const std::string& key, std::string value)
    {
        data[key].push_back(value);
        return *this;
    }

    json_t& add(const std::string& key, uint64_t value)
    {
        data[key].push_back(std::to_string(value));
        return *this;
    }

    json_t& add(const std::string& key, const json_t& value)
    {
        data[key].push_back(value.to_string());
        return *this;
    }

    void append(const std::string& key, std::string value)
    {
        data[key].push_back(value);
    }

    void append(const std::string& key, const json_t &value)
    {
        data[key].push_back(value.to_string());
    }

    std::string to_string() const {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto &pair : data) {
            if (!first)
                oss << ",";

            oss << "\"" << pair.first << "\":";

            if (pair.second.size() == 1)
            {
                oss << "\"" << pair.second.front() << "\"";
            } else {
                oss << "[";
                for (int i = 0; i < pair.second.size(); i++)
                {
                    oss << "\"" << pair.second[i] << "\"";
                    if (i != pair.second.size() - 1)
                        oss << ",";
                }

                oss << "]";
            }

            first = false;
        }
        oss << "}";
        return oss.str();
    }

private:
    std::unordered_map<std::string, std::vector<std::string>> data;
};

#endif /* json_t_UTIL */