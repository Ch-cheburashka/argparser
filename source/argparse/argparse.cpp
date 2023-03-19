//
// Created by Ch-cheburashka on 16.03.2023.
//

#include <argparse/argparse.hpp>
#include <filesystem>

constexpr char *DEFAULT_STR = "default";

std::map<std::string, std::string> string_tokenizing(int argc, char **argv) {
    std::map<std::string, std::string> tokens;
    for (auto i = 1; i < argc; i++) {
        if (i != argc - 1 && argv[i + 1][0] != '-')
            tokens.emplace(argv[i], argv[i + 1]);
        else
            tokens.emplace(argv[i], DEFAULT_STR);
    }
    return tokens;
}

template<typename T>
bool compare_types(const typed_value &any) {
    if (any.m_default_value.type() == typeid(T))
        return true;
    return false;
}

bool option_exists(int index, char** argv, const std::vector<argparse::option> &options) {
    return std::find_if(options.begin(), options.end(), [&index, argv](const argparse::option &op) {
        return op.long_name == argv[index] || op.short_name == argv[index];
    }) != options.end();
}

void check_for_required(std::vector<argparse::option>& options, int argc, char **argv) {
    bool found = false;
    std::string required_options;
    for (auto && option : options) {
        found = false;
        for (auto i = 1; i < argc; ++i) {
            if (option.required) {
                if (argv[i] == option.long_name || argv[i] == option.short_name)
                    found = true;
                if (required_options.find(option.long_name) == std::string::npos)
                    required_options += option.long_name + "(" + option.short_name + ")" + " ";
            }
        }
    }
    if (!found)
        throw std::logic_error("\n\tlack of required arguments\n\trequired arguments are: " + required_options);
}

std::map<std::string, typed_value> argparse::parse_args(int argc, char **argv) {
    std::map<std::string, typed_value> options_map;
    auto tokens = string_tokenizing(argc, argv);
    check_for_required(options,argc,argv);
    for (auto i = 1; i < argc;) {
        for (auto &&option: options) {
            if (tokens.at(argv[i]) == DEFAULT_STR) {
                if (option.long_name != argv[i] && option.short_name != argv[i]) {
                    if (option_exists(i, argv, options))
                        continue;
                    else
                        throw std::logic_error(strcat(argv[i], " is an invalid argument"));
                }
                options_map.emplace(option.long_name.substr(2), option.default_value);
                i++;
                break;
            }
            else {
                if (option.long_name != argv[i] && option.short_name != argv[i]) {
                    if (option_exists(i, argv, options))
                        continue;
                    else
                        throw std::logic_error(strcat(argv[i], " is an invalid argument\n"));
                }
                if (compare_types<int>(option.default_value))
                    options_map.emplace(option.long_name.substr(2),
                                        value<int>().default_value(std::stoi(tokens.at(argv[i]))));
                else if (compare_types<double>(option.default_value))
                    options_map.emplace(option.long_name.substr(2),
                                        value<double>().default_value(std::stod(tokens.at(argv[i]))));
                else if (compare_types<bool>(option.default_value))
                    options_map.emplace(option.long_name.substr(2),
                                        value<bool>().default_value(std::stoi(tokens.at(argv[i]))));
                else if (compare_types<std::filesystem::path>(option.default_value))
                    options_map.emplace(option.long_name.substr(2),
                                        value<std::filesystem::path>().default_value(
                                                std::filesystem::path(tokens.at(argv[i]))));
                else
                    options_map.emplace(option.long_name.substr(2),
                                        value<std::string>().default_value(std::string(tokens.at(argv[i]))));
                i += 2;
                break;
            }
        }
    }
    return options_map;
}

void argparse::add_option(const argparse::option &config) {
    options.emplace_back(config);
}

void argparse::print_options() {
    std::cout << _options_description << ":\n";
    for (auto &&option: options) {
        std::cout << "  " << option.long_name << "(" << option.short_name << ")\t" << option.help << "\n";
    }
}