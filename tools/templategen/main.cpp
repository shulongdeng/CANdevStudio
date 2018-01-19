#include <cxxopts.hpp>
#include <spdlog/fmt/fmt.h>

namespace {

struct FuncDefAdder {
    FuncDefAdder(const std::string& className, const std::string& base = "")
    {
        using namespace fmt::literals;

        _buff += fmt::format("class {name}{base} {{\n",
                "name"_a = className,
                "base"_a = base.length() ? fmt::format(" : {}", base) : "");
    }
    
    FuncDefAdder& privateSection()
    {
        _buff += "private:\n";

        return *this;
    }

    FuncDefAdder& signalsSection()
    {
        _buff += "signals:\n";

        return *this;
    }

    FuncDefAdder& slotsSection()
    {
        _buff += "slots:\n";

        return *this;
    }

    FuncDefAdder& publicSection()
    {
        _buff += "public:\n";

        return *this;
    }

    FuncDefAdder& operator()(const std::string& line)
    {
        _buff += line;

        return *this;
    }

    FuncDefAdder& operator()(const std::string& type, const std::string& name)
    {
        using namespace fmt::literals;

        _buff += fmt::format("{indent} {type} {name};\n", 
                "indent"_a = "    ",
                "type"_a = type,
                "name"_a = name);

        return *this;
    }

    FuncDefAdder& operator()(const std::string& name, const std::string& ret, const std::string& args,
        bool over = false, bool constant = false)
    {
        using namespace fmt::literals;

        // clang-format off
        _buff += fmt::format("{indent}{ret} {name}({args}){override}{const};\n", 
                "ret"_a = ret,
                "name"_a = name,
                "args"_a = args,
                "override"_a = over ? " override" : "",
                "const"_a = constant ? " const" : "",
                "indent"_a = "    ");
        // clang-format on

        return *this;
    }

    std::string get()
    {
        if(!_finished) {
            _buff += "};\n";
            _finished = true;
        }

        return _buff;
    }

private:
    std::string _buff;
    bool _finished{ false };
};

} // namespace

int main(int argc, char* argv[])
{
    cxxopts::Options options(argv[0], "CANdevStudio component template generator");
    // options.custom_help("-n NewComponent -o ~/CANdevStudio/src/components/newcomponent [OPTIONS...]");

    // clang-format off
    options.add_options()
    ("n, name", "Component name", cxxopts::value<std::string>(), "name")
    ("o, output", "Output folder", cxxopts::value<std::string>(), "path")
    ("g, no-gui", "Component without GUI", cxxopts::value<bool>())
    ("h,help", "Show help message");
    // clang-format on

    const auto&& result = options.parse(argc, argv);

    if (result.count("h")) {
        std::cout << options.help({ "" }) << std::endl;
        return EXIT_FAILURE;
    }

    if (result.count("n") == 0 || result.count("o") == 0) {
        std::cerr << options.help({ "" }) << std::endl;
        return EXIT_FAILURE;
    }

    // clang-format off
    std::cout << FuncDefAdder("SomeClass")
        .publicSection()
        ("blahFunc", "void", "int i", true, true)
        ("blahFunc2", "std::string", "int a, int b")
        .privateSection()
        ("das", "dasda", "dsdds")
        .get();
    // clang-format on

    return EXIT_SUCCESS;
}
