#include <boost/filesystem.hpp>
#include <cxxopts.hpp>
#include <spdlog/fmt/fmt.h>

namespace {

struct ComponentGenerator {
    ComponentGenerator(const std::string& className, const std::string& base = "")
        : _className(className)
    {
        using namespace fmt::literals;

        _hdrBuff += fmt::format("class {name}{base} {{\n", "name"_a = className,
            "base"_a = base.length() ? fmt::format(" : {}", base) : "");

        _srcBuff += fmt::format(R"(
{className}::{className}()
{{
}}
)",
            "className"_a = _className);

        _srcBuff += fmt::format(R"(
{className}::~{className}()
{{
}}
)",
            "className"_a = _className);
    }

    ComponentGenerator& privateSection()
    {
        _hdrBuff += "private:\n";

        return *this;
    }

    ComponentGenerator& signalsSection()
    {
        _hdrBuff += "signals:\n";

        return *this;
    }

    ComponentGenerator& slotsSection()
    {
        _hdrBuff += "public slots:\n";

        return *this;
    }

    ComponentGenerator& publicSection()
    {
        _hdrBuff += "public:\n";

        return *this;
    }

    ComponentGenerator& newLine()
    {
        _hdrBuff += "\n";

        return *this;
    }

    ComponentGenerator& operator()(const std::string& line)
    {
        _hdrBuff += line + "\n";

        return *this;
    }

    ComponentGenerator& operator()(const std::string& type, const std::string& name)
    {
        using namespace fmt::literals;

        _hdrBuff += fmt::format("{indent}{type} {name};\n", "indent"_a = "    ", "type"_a = type, "name"_a = name);

        return *this;
    }

    ComponentGenerator& operator()(const std::string& name, const std::string& ret, const std::string& args,
        bool over = false, bool constant = false)
    {
        using namespace fmt::literals;

        // clang-format off
        _hdrBuff += fmt::format("{indent}{ret} {name}({args}){const}{override};\n", 
                "ret"_a = ret,
                "name"_a = name,
                "args"_a = args,
                "override"_a = over ? " override" : "",
                "const"_a = constant ? " const" : "",
                "indent"_a = "    ");
        // clang-format on

        _srcBuff += fmt::format(R"(
{ret} {className}::{funcName}({args}){const}
{{
    return{retVal};
}}
)",
            "ret"_a = ret, "funcName"_a = name, "className"_a = _className, "args"_a = args,
            "const"_a = constant ? " const" : "", "retVal"_a = ret == "void" ? "" : "{}");

        return *this;
    }

    std::string getSrc()
    {
        return _srcBuff;
    }

    std::string getHdr()
    {
        if (!_finished) {
            _hdrBuff += "};\n";
            _finished = true;
        }

        return _hdrBuff;
    }

private:
    std::string _srcBuff;
    std::string _hdrBuff;
    std::string _className;
    bool _finished{ false };
};

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string str_toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
    return s;
}

std::pair<std::string, std::string> genComponentImpl(const std::string& componentName)
{
    std::string srcBuff;
    std::string hdrBuff;
    auto componentNameUpper = str_toupper(componentName);

    // clang-format off
    ComponentGenerator classDesc(componentName, "public QObject, public ComponentInterface");

    classDesc
        ("    Q_OBJECT")
        ("    Q_DECLARE_PRIVATE(" + componentName + ")")
        .newLine()
        .publicSection()
        ("    " + componentName + "();")
        ("    explicit " + componentName + "(" + componentName + "Ctx&& ctx);")
        ("    ~" + componentName + "();")
        .newLine()
        ("mainWidget", "QWidget*", "", true)
        ("setConfig", "void", "const QJsonObject& json", true)
        ("setConfig", "void", "const QObject& qobject", true)
        ("getConfig", "QJsonObject", "", true, true)
        ("getQConfig", "std::shared_ptr<QObject>", "", true, true)
        ("configChanged", "void", "", true)
        ("mainWidgetDocked", "bool", "", true, true)
        ("getSupportedProperties", "ComponentInterface::ComponentProperties", "", true, true)
        .newLine()
        .signalsSection()
        ("mainWidgetDockToggled", "void", "QWidget* widget", true)
        .newLine()
        .slotsSection()
        ("stopSimulation", "void", "", true)
        ("startSimulation", "void", "", true)
        .newLine()
        .privateSection()
        ("QScopedPointer<" + componentName + "Private>","d_ptr");
    // clang-format on

    hdrBuff += "#ifndef " + componentNameUpper + "_H\n";
    hdrBuff += "#define " + componentNameUpper + "_H\n\n";

    hdrBuff += "#include <QtCore/QObject>\n";
    hdrBuff += "#include <QtCore/QScopedPointer>\n";
    hdrBuff += "#include <componentinterface.h>\n";
    hdrBuff += "#include <context.h>\n";
    hdrBuff += "#include <memory>\n\n";

    hdrBuff += "class " + componentName + "Private;\n";
    hdrBuff += "class QWidget;\n\n";

    hdrBuff += classDesc.getHdr();

    hdrBuff += "\n#endif //" + componentNameUpper + "_H\n";

    srcBuff += "#include \"" + str_tolower(componentName) + ".h\"\n";
    srcBuff += classDesc.getSrc();

    return { hdrBuff, srcBuff };
}

std::string genCMake(const std::string name)
{
    using namespace fmt::literals;

    return fmt::format(R"(set(COMPONENT_NAME {name})

set(SRC
#    gui/{name}.ui
    {name}.cpp
)

add_library(${{COMPONENT_NAME}} ${{SRC}})
target_link_libraries(${{COMPONENT_NAME}} Qt5::Widgets Qt5::Core Qt5::SerialBus nodes cds-common)
target_include_directories(${{COMPONENT_NAME}} INTERFACE ${{CMAKE_CURRENT_SOURCE_DIR}})
)",
        "name"_a = name);
}

} // namespace

int main(int argc, char* argv[])
{
    cxxopts::Options options(argv[0], "CANdevStudio component template generator");

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

    auto componentName = result["n"].as<std::string>();
    auto componentPath = result["o"].as<std::string>();
    auto componentNameLower = str_tolower(componentName);

    boost::filesystem::path componentDir(componentPath);
    if (!boost::filesystem::exists(componentDir)) {
        if (!boost::filesystem::create_directory(componentDir)) {
            std::cerr << "Failed to create output directory '" << componentPath << "'" << std::endl;

            return EXIT_FAILURE;
        }
    }

    boost::filesystem::ofstream cmakeFile({ componentDir / "CMakeLists.txt" });
    cmakeFile << genCMake(componentNameLower);
    cmakeFile.close();

    auto compImpl = genComponentImpl(componentName);

    boost::filesystem::ofstream compHdrFile({ componentDir / (componentNameLower + ".h") });
    compHdrFile << compImpl.first;
    compHdrFile.close();

    boost::filesystem::ofstream compSrcFile({ componentDir / (componentNameLower + ".cpp") });
    compSrcFile << compImpl.second;
    compSrcFile.close();

    return EXIT_SUCCESS;
}
