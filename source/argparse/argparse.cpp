//
// Created by Ch-cheburashka on 16.03.2023.
//

#include <argparse/argparse.hpp>
#include <filesystem>

std::map<std::string,typed_value> argparse::parse_args(int argc, char **argv) {
    std::map<std::string,typed_value> options_map;
    for (auto i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                for (auto &&option: options) {
                    if (option.long_name == argv[i]) {
                        if (i != argc - 1 && argv[i+1][0] != '-') {
                            if (option.default_value.m_default_value.type() == typeid(int))
                                options_map.emplace(option.long_name.substr(2),
                                                    value<int>().default_value(std::stoi(argv[i + 1])));
                            else if (option.default_value.m_default_value.type() == typeid(double))
                                options_map.emplace(option.long_name.substr(2),
                                                    value<double>().default_value(std::stod(argv[i + 1])));
                            else if (option.default_value.m_default_value.type() == typeid(bool))
                                options_map.emplace(option.long_name.substr(2),
                                                    value<bool>().default_value(std::stoi(argv[i + 1])));
                            else if (option.default_value.m_default_value.type() == typeid(std::filesystem::path))
                                options_map.emplace(option.long_name.substr(2),
                                                    value<std::filesystem::path>().default_value(std::filesystem::path(argv[i + 1])));
                        }
                        else {
                            options_map.emplace(option.long_name.substr(2),
                                                option.default_value);
                        }
                    }
                }
            }
            else {
                for (auto &&option: options) {
                    if (option.short_name == argv[i]) {
                        if (i != argc - 1 && argv[i+1][0] != '-') {
                            if (option.default_value.m_default_value.type() == typeid(int))
                                options_map.emplace(option.long_name.substr(2),
                                                    value<int>().default_value(std::stoi(argv[i + 1])));
                            else if (option.default_value.m_default_value.type() == typeid(double))
                                options_map.emplace(option.long_name.substr(2),
                                                    value<double>().default_value(std::stod(argv[i + 1])));
                            else if (option.default_value.m_default_value.type() == typeid(bool))
                                options_map.emplace(option.long_name.substr(2),
                                                    value<bool>().default_value(std::stoi(argv[i + 1])));
                            else if (option.default_value.m_default_value.type() == typeid(std::filesystem::path))
                                options_map.emplace(option.long_name.substr(2),
                                                    value<std::filesystem::path>().default_value(std::filesystem::path(argv[i + 1])));
                        }
                        else {
                            options_map.emplace(option.long_name.substr(2),
                                                option.default_value);
                        }
                    }
                }
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

