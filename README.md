# Библиотека для парсинга аргументов командной строки - `argparse`

## Задача

Написать библиотеку на C++17, которая будет предоставлять api для удобного объявления, считывания и парсинга параметров командной строки


## Подсказка для реализации

```c++
#include <iostream>
#include <vector>
#include <any>

// Класс, который содержит 
// итоговое дефолтное значение,
// скрыт от конечного пользователя
struct typed_value {
    std::any m_default_value;

    template<typename T>
    T get_value() {
        return std::any_cast<T>(m_default_value);
    }

};

// Класс, который впоследствии будет
// использоваться пользователем
template<typename T>
struct value : public typed_value {
    value() = default;

    value<T> default_value(T &&v) {
        this->m_default_value = v;
        return *this;
    }
};

struct option {
    std::string short_name;
    std::string long_name;
    bool required;
    typed_value default_value;
    std::string help;
};

std::vector<option> options;

void add_option(option config) {
    options.emplace_back(std::move(config));
}

void print_options() {
    for (auto &&option: options) {
        std::cout << option.default_value.get_value<int>();
    }
}

int main() {
    // Пример использования. Референс на boost::program_options
    add_option(option{
            .short_name="-c", .long_name="--count",
            .required=false, .default_value=value<int>().default_value(10),
            .help="Number of times to count"
    });

    print_options();
    return 0;
}

```

## Примерный кейс использования
```c++
#include <argparse.hpp>
#include <memory>

int main(int argc, char **argv) {
    argparse::arguments_parser parser("Allowed options");
    parser.add_option(argparse::option{
            .short_name="-c", .long_name="--count",
            .required=false, .default_value=value<int>().default_value(10),
            .help="Number of times to count"
        });
    parser.add_option(argparse::option{
            .short_name="-o", .long_name="--output",
            .required=false, .default_value=value<std::filesystem::path>().default_value(10),
            .help="Number of times to count"
        });
    
    auto arguments = parser.parse_args(argc, argv);
    
    std::unique_ptr<std::ostream> stream;
    
    if (arguments.exists("output")) {
        stream = make_unique<std::ofstream>(arguments.get("output"));
    } else {
        stream = make_unique<std::ostream>(std::cout.rdbuf());
    }
    
    for (int i = 0; i < arguments.get("count").length(); i++) {
        (*stream) << i << '\n';
    }
}
```