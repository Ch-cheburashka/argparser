//
// Created by Ch-cheburashka on 16.03.2023.
//

#include <argparse/argparse.hpp>

std::map<std::string,typed_value> argparse::parse_args(int argc, char **argv) {
    std::map<std::string,typed_value> options_map;
    for (auto i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                for (auto &&option: options) {
                    if (option.long_name == argv[i]) {
                        if (argc == 2) {
                            options_map.emplace(option.long_name.substr(2),
                                                option.default_value);
                        }
                        else
                            options_map.emplace(option.long_name.substr(2),
                                                value<std::string>().default_value(argv[i + 1]));
                    }
                }
            }
            else {
                for (auto &&option: options) {
                    if (option.short_name == argv[i]) {
                        if (argc == 2) {
                            options_map.emplace(option.long_name.substr(2),
                                                option.default_value);
                        } else {
                            options_map.emplace(option.long_name.substr(2),
                                                value<std::string>().default_value(argv[i + 1]));
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

