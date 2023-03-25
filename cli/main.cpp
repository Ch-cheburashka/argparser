#include <iostream>
#include <argparse/argparse.hpp>
#include <filesystem>

int main(int argc, char **argv) {

    argparse parser{"Allowed options"};

    parser.add_option(argparse::option{
            .short_name="-c", .long_name="--count",
            .required=true, .default_value=value<int>().default_value(10),
            .help="Set number of times to count"
    });

    parser.add_option(argparse::option{
            .short_name="-o", .long_name="--output",
            .required=false, .default_value=value<std::filesystem::path>().default_value(
                    std::filesystem::current_path()),
            .help="Set path to output"
    });

    parser.add_option(argparse::option{
            .short_name = "-h", .long_name = "--help",
            .required=false, .help="Print help message"
    });

    parser.add_option(argparse::option{
            .short_name = "-g", .long_name = "--greet",
            .required = true, .default_value=value<std::string>().default_value("moron"),
            .help = "Set name to greet"
    });

    auto arguments = parser.parse_args(argc, argv);

    if (arguments.count("greet")) {
        std::cout << "Hello, " << arguments.at("greet").get_value<std::string>() << "!\n";
    }
    if (arguments.count("count")) {
        std::cout << "Number of times to count was set to " << arguments.at("count").get_value<int>() << "\n";
        for (auto i = 1; i <= arguments.at("count").get_value<int>(); ++i) {
            std::cout << "number: " << i << "\n";
        }
    }

    if (arguments.count("output")) {
        std::cout << "Path to output was set to " << arguments.at("output").get_value<std::filesystem::path>() << "\n";
        if (exists(arguments.at("output").get_value<std::filesystem::path>())) {
            std::cout << "path exists\n";
        } else
            std::cout << "path DOESN'T exist\n";
    }

    if (arguments.count("help")) {
        parser.print_options();
        return 1;
    }

    return 0;
}