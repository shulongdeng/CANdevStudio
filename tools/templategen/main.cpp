#include <boost/filesystem.hpp>
#include <cxxopts.hpp>
#include <spdlog/fmt/fmt.h>

namespace {

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

std::string genCMake(const std::string name)
{
    using namespace fmt::literals;

    return fmt::format(R"(set(COMPONENT_NAME {name})

set(SRC
#    gui/{name}.ui
    {name}.cpp
    {name}_p.cpp
)

add_library(${{COMPONENT_NAME}} ${{SRC}})
target_link_libraries(${{COMPONENT_NAME}} Qt5::Widgets Qt5::Core Qt5::SerialBus nodes cds-common)
target_include_directories(${{COMPONENT_NAME}} INTERFACE ${{CMAKE_CURRENT_SOURCE_DIR}})
)",
        "name"_a = name);
}


std::string genGuiComponentHdr(const std::string& name)
{
    using namespace fmt::literals;

    return fmt::format(R"(#ifndef {nameUpper}_H
#define {nameUpper}_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <componentinterface.h>
#include <context.h>
#include <memory>

class {name}Private;
class QWidget;
struct I{name}Gui;
typedef Context<I{name}Gui> {name}Ctx;

class {name} : public QObject, public ComponentInterface {{
    Q_OBJECT
    Q_DECLARE_PRIVATE({name})

public:
    {name}();
    explicit {name}({name}Ctx&& ctx);
    ~{name}();

    QWidget* mainWidget() override;
    void setConfig(const QJsonObject& json) override;
    void setConfig(const QObject& qobject) override;
    QJsonObject getConfig() const override;
    std::shared_ptr<QObject> getQConfig() const override;
    void configChanged() override;
    bool mainWidgetDocked() const override;
    ComponentInterface::ComponentProperties getSupportedProperties() const override;

signals:
    void mainWidgetDockToggled(QWidget* widget) override;

public slots:
    void stopSimulation() override;
    void startSimulation() override;

private:
    QScopedPointer<{name}Private> d_ptr;
}};

#endif //{nameUpper}_H
)",
    "name"_a = name, "nameUpper"_a = str_toupper(name));

}

std::string genGuiComponentSrc(const std::string& name)
{
    using namespace fmt::literals;

    return fmt::format(R"(#include "{nameLower}.h"
#include "{nameLower}_p.h"

{name}::{name}()
    : d_ptr(new {name}Private(this))
{{
}}

{name}::~{name}()
{{
}}

QWidget* {name}::mainWidget()
{{
    return {{ }};
}}

void {name}::setConfig(const QJsonObject& json)
{{
}}

void {name}::setConfig(const QObject& qobject)
{{
}}

QJsonObject {name}::getConfig() const
{{
    return {{ }};
}}

std::shared_ptr<QObject> {name}::getQConfig() const
{{
    return {{ }};
}}

void {name}::configChanged()
{{
}}

bool {name}::mainWidgetDocked() const
{{
}}

ComponentInterface::ComponentProperties {name}::getSupportedProperties() const
{{
}}

void {name}::mainWidgetDockToggled(QWidget* widget)
{{
}}

void {name}::stopSimulation()
{{
}}

void {name}::startSimulation()
{{
}})", "name"_a = name, "nameLower"_a = str_tolower(name));
}

std::string genGuiPrivateHdr(const std::string& name)
{
    using namespace fmt::literals;

    return fmt::format(R"(#ifndef {nameUpper}_P_H
#define {nameUpper}_P_H

#include <QtCore/QObject>
#include <memory>

class {name};

class {name}Private : public QObject {{
    Q_OBJECT
    Q_DECLARE_PUBLIC({name})

public:
    {name}Private({name}* q);

private:
    {name}* q_ptr;
}};

#endif // {nameUpper}_P_H
)",
    "name"_a = name, "nameUpper"_a = str_toupper(name));

}

std::string genGuiPrivateSrc(const std::string& name)
{
    using namespace fmt::literals;

    return fmt::format(R"(#include "{nameLower}_p.h"

{name}Private::{name}Private({name} *q)
    : q_ptr(q)
{{
}}
)", "name"_a = name, "nameLower"_a = str_tolower(name));
}

void writeToFile(const boost::filesystem::path& filename, const std::string& content)
{
    boost::filesystem::ofstream file(filename);
    file << content;
    file.close();
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

    writeToFile(componentDir / "CMakeLists.txt", genCMake(componentNameLower));
    writeToFile(componentDir / (componentNameLower + ".h"), genGuiComponentHdr(componentName));
    writeToFile(componentDir / (componentNameLower + ".cpp"), genGuiComponentSrc(componentName));
    writeToFile(componentDir / (componentNameLower + "_p.h"), genGuiPrivateHdr(componentName));
    writeToFile(componentDir / (componentNameLower + "_p.cpp"), genGuiPrivateSrc(componentName));

    return EXIT_SUCCESS;
}
