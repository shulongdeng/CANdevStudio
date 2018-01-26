set(COMPONENT_NAME cansignalsender)

set(SRC
    gui/cansignalsender.ui
    gui/cansignalsenderguiimpl.h
    cansignalsender.cpp
    cansignalsender_p.cpp
)

add_library(${COMPONENT_NAME} ${SRC})
target_link_libraries(${COMPONENT_NAME} Qt5::Widgets Qt5::Core Qt5::SerialBus nodes cds-common)
target_include_directories(${COMPONENT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
