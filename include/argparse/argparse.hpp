//
// Created by CH-cheburashka on 16.03.2023.
//

#ifndef ARGPARSER_ARGPARSE_HPP
#define ARGPARSER_ARGPARSE_HPP

#include <any>
#include <vector>
#include <map>
#include <iostream>
#include <cstring>
#include<typeindex>

struct typed_value {
    std::any m_default_value;

    template<typename T>
    T get_value() {
        return std::any_cast<T>(m_default_value);
    }
};

template<typename T>
struct value : public typed_value {
    value() = default;

    value<T> default_value(T &&v) {
        this->m_default_value = v;
        return *this;
    }
};

class argparse {
private:
    std::string _options_description;
public:
    explicit argparse(std::string options_description) : _options_description(std::move(options_description)) {}
    struct option {
        std::string short_name;
        std::string long_name;
        bool required;
        typed_value default_value;
        std::string help;
    };
    std::vector<option> options;
    void add_option(const option& config);
    void print_options () ;
    std::map<std::string,typed_value> parse_args (int argc, char** argv);
};

#endif //ARGPARSER_ARGPARSE_HPP
