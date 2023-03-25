//
// Created by Ch-cheburashka on 16.03.2023.
//

#include <argparse/argparse.hpp>
#include <filesystem>

constexpr char *default_str = "default";
constexpr char option_mark = '-';

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

std::map<std::string, std::string> tokenize_args(int argc, char **argv, std::vector<argparse::option> required) {
    std::map<std::string, std::string> tokens;
    std::string current_option = "";
    for (auto i = 1; i < argc; ++i) {
        if (argv[i][0] == option_mark && current_option.empty())
            current_option = argv[i];
        else if (argv[i][0] != option_mark) {
            tokens.emplace(current_option, std::string(argv[i]));
            current_option.clear();
        }
        else  {
            tokens.emplace(current_option,default_str);
            current_option.clear();
            i--;
        }
        required.erase(std::remove_if(required.begin(), required.end(), [current_option](auto opt){
            return opt.short_name == current_option || opt.long_name == current_option || !opt.required;
        }),required.end());
    }
    if (!current_option.empty())
        tokens.emplace(current_option,default_str);
    if (!required.empty()) {
        std::stringstream error_stream;
        error_stream << "There are some required parameters missing:\n";
        for (auto &option : required) {
            error_stream << option.long_name << "(" + option.short_name + ")" << "\n";
        }
        throw std::logic_error(error_stream.str());
    }
    return tokens;
}

std::map<std::string, typed_value> argparse::parse_args(int argc, char **argv) {
    std::map<std::string, typed_value> options_map;
    auto tokens = tokenize_args(argc, argv,options);
    for (auto i = 1; i < argc;) {
        for (auto &&option: options) {
            if (tokens.at(argv[i]) == default_str) {
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