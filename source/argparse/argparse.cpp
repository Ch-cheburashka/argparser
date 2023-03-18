//
// Created by Ch-cheburashka on 16.03.2023.
//

#include <argparse/argparse.hpp>
#include <filesystem>

constexpr char* DEFAULT_STR = "default";

std::map<std::string,std::string> string_tokenizing(int argc, char** argv) {
    std::map<std::string,std::string> tokens;
    for (auto i = 1; i < argc; i++) {
        if (i != argc - 1 && argv[i+1][0] != '-')
            tokens.emplace(argv[i],argv[i+1]);
        else
            tokens.emplace(argv[i],DEFAULT_STR);
    }
    return tokens;
}

std::map<std::string,typed_value> argparse::parse_args(int argc, char **argv) {
    std::map<std::string,typed_value> options_map;
    auto tokens = string_tokenizing(argc,argv);

    for (auto i = 1; i < argc;) {
        for (auto &&option: options) {
            if (tokens.at(argv[i]) == DEFAULT_STR) {
                if (option.long_name != argv[i] && option.short_name != argv[i])
                    continue;
                options_map.emplace(option.long_name.substr(2),
                                    option.default_value);
                i++;
                break;
            }
            else {
                if (option.long_name != argv[i] && option.short_name != argv[i])
                    continue;
                if (option.default_value.m_default_value.type() == typeid(int))
                    options_map.emplace(option.long_name.substr(2),
                                        value<int>().default_value(std::stoi(tokens.at(argv[i]))));
                else if (option.default_value.m_default_value.type() == typeid(double))
                    options_map.emplace(option.long_name.substr(2),
                                        value<double>().default_value(std::stod(tokens.at(argv[i]))));
                else if (option.default_value.m_default_value.type() == typeid(bool))
                    options_map.emplace(option.long_name.substr(2),
                                        value<bool>().default_value(std::stoi(tokens.at(argv[i]))));
                else if (option.default_value.m_default_value.type() == typeid(std::filesystem::path))
                    options_map.emplace(option.long_name.substr(2),
                                        value<std::filesystem::path>().default_value(std::filesystem::path(tokens.at(argv[i]))));
                i+=2;
                break;
            }
        }
    }
    return options_map;
}

void argparse::add_option(const argparse::option& config) {
    options.emplace_back(config);
}

void argparse::print_options() {
    std::cout << options_description << ":\n";
    for (auto &&option: options) {
        std::cout << "  " << option.long_name << "(" << option.short_name << ")\t" << option.help << "\n";
    }
}

