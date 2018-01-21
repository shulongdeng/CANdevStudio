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
    gui/{name}.ui
    gui/{name}guiimpl.h
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
struct {name}GuiInt;
typedef Context<{name}GuiInt> {name}Ctx;

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

{name}::{name}({name}Ctx&& ctx)
    : d_ptr(new {name}Private(this, std::move(ctx)))
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
    (void)json;
}}

void {name}::setConfig(const QObject& qobject)
{{
    (void)qobject;
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
    return d_ptr->_docked;
}}

ComponentInterface::ComponentProperties {name}::getSupportedProperties() const
{{
    return {{ }};
}}

void {name}::stopSimulation()
{{
}}

void {name}::startSimulation()
{{
}}
)", "name"_a = name, "nameLower"_a = str_tolower(name));
}

std::string genGuiPrivateHdr(const std::string& name)
{
    using namespace fmt::literals;

    return fmt::format(R"(#ifndef {nameUpper}_P_H
#define {nameUpper}_P_H

#include <QtCore/QObject>
#include <memory>
#include "gui/{nameLower}guiimpl.h"
#include "{nameLower}.h"

class {name};

class {name}Private : public QObject {{
    Q_OBJECT
    Q_DECLARE_PUBLIC({name})

public:
    {name}Private({name}* q, {name}Ctx&& ctx = {name}Ctx(new {name}GuiImpl));

    bool _docked {{ true }};

private:
    {name}* q_ptr;
    {name}Ctx _ctx;
}};

#endif // {nameUpper}_P_H
)",
    "name"_a = name, "nameUpper"_a = str_toupper(name), "nameLower"_a = str_tolower(name));

}

std::string genGuiPrivateSrc(const std::string& name)
{
    using namespace fmt::literals;

    return fmt::format(R"(#include "{nameLower}_p.h"

{name}Private::{name}Private({name} *q, {name}Ctx&& ctx)
    : q_ptr(q)
    , _ctx(std::move(ctx))
{{
}}
)", "name"_a = name, "nameLower"_a = str_tolower(name));
}

std::string genGuiInt(const std::string& name)
{
    using namespace fmt::literals;

    return fmt::format(R"(#ifndef {nameUpper}GUIINT_H
#define {nameUpper}GUIINT_H

#include <Qt>
#include <functional>

class QWidget;

struct {name}GuiInt {{
    virtual ~{name}GuiInt()
    {{
    }}

    virtual QWidget* mainWidget() = 0;
}};

#endif // {nameUpper}GUIINT_H
)", "name"_a = name, "nameUpper"_a = str_toupper(name));
}

std::string genGuiImpl(const std::string& name)
{
    using namespace fmt::literals;

    return fmt::format(R"(#ifndef {nameUpper}GUIIMPL_H
#define {nameUpper}GUIIMPL_H

#include <QWidget>
#include "{nameLower}guiint.h"
#include "ui_{nameLower}.h"

struct {name}GuiImpl : public {name}GuiInt {{
    {name}GuiImpl()
        : _ui(new Ui::{name})
        , _widget(new QWidget)
    {{
    }}

    virtual QWidget* mainWidget()
    {{
        return _widget;
    }}

private:
    Ui::{name}* _ui;
    QWidget* _widget;
}};

#endif // {nameUpper}GUIIMPL_H
)", "name"_a = name, "nameUpper"_a = str_toupper(name), "nameLower"_a = str_tolower(name));
}

std::string genGui(const std::string& name)
{
    using namespace fmt::literals;

    return fmt::format(R"(<ui version="4.0" >
 <author></author>
 <comment></comment>
 <exportmacro></exportmacro>
 <class>{name}</class>
 <widget class="QWidget" name="{name}" >
  <property name="geometry" >
   <rect>
    <x>0</x>
    <y>0</y>
    <width>400</width>
    <height>300</height>
   </rect>
  </property>
  <property name="windowTitle" >
   <string>{name}</string>
  </property>
 </widget>
 <pixmapfunction></pixmapfunction>
 <connections/>
</ui>
)", "name"_a = name);
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

    boost::filesystem::path componentGuiDir(componentPath + "/gui");
    if (!boost::filesystem::exists(componentGuiDir)) {
        if (!boost::filesystem::create_directory(componentGuiDir)) {
            std::cerr << "Failed to create output directory '" << componentPath << "/gui'" << std::endl;

            return EXIT_FAILURE;
        }
    }

    writeToFile(componentDir / "CMakeLists.txt", genCMake(componentNameLower));
    writeToFile(componentDir / (componentNameLower + ".h"), genGuiComponentHdr(componentName));
    writeToFile(componentDir / (componentNameLower + ".cpp"), genGuiComponentSrc(componentName));
    writeToFile(componentDir / (componentNameLower + "_p.h"), genGuiPrivateHdr(componentName));
    writeToFile(componentDir / (componentNameLower + "_p.cpp"), genGuiPrivateSrc(componentName));

    writeToFile(componentGuiDir / (componentNameLower + "guiimpl.h"), genGuiImpl(componentName));
    writeToFile(componentGuiDir / (componentNameLower + "guiint.h"), genGuiInt(componentName));
    writeToFile(componentGuiDir / (componentNameLower + ".ui"), genGui(componentName));

    return EXIT_SUCCESS;
}
