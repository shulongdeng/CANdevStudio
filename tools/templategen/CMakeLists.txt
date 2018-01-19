include_directories(${CMAKE_SOURCE_DIR}/3rdParty/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/cxxopts/include)

set(SRC
    main.cpp
    )

add_executable(templategen ${SRC})

