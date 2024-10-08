cmake_minimum_required(VERSION 3.20)

project(OpenChickenInvaders)

option(USE_SFML "use SFML (otherwise SDL will be used" OFF)
option(USE_IWYU "enable include-what-you-use tool" OFF)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE headers "src/*.h")

set(sources
    src/audio/controller_holder.cpp
    src/audio/player.cpp
    src/background/background.cpp
    src/constants.cpp
    src/context/context.cpp
    src/context/manager.cpp
    src/core/color.cpp
    src/core/image.cpp
    src/core/keyboard.cpp
    src/core/mouse.cpp
    src/core/rectangle_shape.cpp
    src/core/sprite.cpp
    src/core/texture.cpp
    src/core/window.cpp
    src/diagnostics/benchmark/benchmark.cpp
    src/diagnostics/fps.cpp
    src/egg.cpp
    src/font/font.cpp
    src/game/state.cpp
    src/levels/factory.cpp
    src/levels/game_level.cpp
    src/levels/game_over.cpp
    src/levels/interface/indicator.cpp
    src/levels/interface/interface.cpp
    src/levels/interface/lives_indicator.cpp
    src/levels/interface/missiles_indicator.cpp
    src/levels/intro_0.cpp
    src/levels/intro_1.cpp
    src/levels/intro_level.cpp
    src/levels/level.cpp
    src/levels/level_1_1.cpp
    src/levels/level_1_10.cpp
    src/levels/level_1_2.cpp
    src/levels/level_1_4.cpp
    src/levels/level_1_5.cpp
    src/levels/level_1_8.cpp
    src/levels/level_1_9.cpp
    src/levels/level_x2_x3.cpp
    src/levels/level_x4.cpp
    src/levels/loader.cpp
    src/levels/manager.cpp
    src/levels/start_round.cpp
    src/main.cpp
    src/menu/background_controller.cpp
    src/menu/cursor.cpp
    src/menu/mainmenu.cpp
    src/objects/base/animated.cpp
    src/objects/base/animated_collision_object.cpp
    src/objects/base/collision_object.cpp
    src/objects/base/sprite.cpp
    src/objects/bonus/bonus.cpp
    src/objects/bonus/chicken_body.cpp
    src/objects/bonus/chicken_leg.cpp
    src/objects/bonus/chicken_part.cpp
    src/objects/bonus/coin.cpp
    src/objects/bonus/switch_gun.cpp
    src/objects/bonus/upgrade_gun.cpp
    src/objects/characters/asteroid.cpp
    src/objects/characters/boss1.cpp
    src/objects/characters/chicken.cpp
    src/objects/characters/jumping_asteroid.cpp
    src/objects/characters/paratrooper.cpp
    src/objects/characters/player_ship.cpp
    src/objects/characters/random_chicken.cpp
    src/objects/characters/shield.cpp
    src/objects/characters/ship.cpp
    src/objects/characters/ufo.cpp
    src/objects/effects/unshade_screen.cpp
    src/objects/gun/bullet.cpp
    src/objects/gun/green_gun.cpp
    src/objects/gun/gun.cpp
    src/objects/gun/missile.cpp
    src/objects/gun/red_gun.cpp
    src/objects/particles/bang.cpp
    src/objects/particles/exhaust.cpp
    src/objects/particles/particle.cpp
    src/objects/particles/smoke.cpp
    src/objects/particles/spark.cpp
    src/objects/text/subtitle_text.cpp
    src/objects/text/text.cpp
    src/resources/loader.cpp
    src/solar_system.cpp
    src/utils/fast_trigonometry.cpp
)

macro(GroupSources curdir)
   file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
   foreach(child ${children})
          if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            GroupSources(${curdir}/${child})
          else()
            string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
          endif()
   endforeach()
endmacro()

GroupSources(src)

add_executable(oci ${sources} ${headers})

#
# Build tinyxml2
#

option(tinyxml2_BUILD_TESTING "" OFF)
include(FetchContent)
FetchContent_Declare(tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG v10.0.0
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(tinyxml2)
target_link_libraries(oci PRIVATE tinyxml2)
target_include_directories(oci PRIVATE ${tinyxml2_SOURCE_DIR})

#
# Download the resource file of the original game
#

find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_custom_command(
	DEPENDS "${CMAKE_SOURCE_DIR}/utility/download_resources.py"
    TARGET oci POST_BUILD
	
	COMMAND ${Python3_EXECUTABLE}
	"${CMAKE_SOURCE_DIR}/utility/download_resources.py"
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)



include(GNUInstallDirs)
if (EMSCRIPTEN)
    if (${USE_SFML})
        message(FATAL_ERROR "Only SDL build is supported for WASM target")
    endif ()

    target_compile_options(oci PRIVATE "-sUSE_SDL=2")
    target_link_options(oci PRIVATE
        "-sASYNCIFY"
        "--use-port=sdl2"
        "--use-port=sdl2_mixer"
        "-sINVOKE_RUN=0"
        "-sEXIT_RUNTIME=1"
        "-sMODULARIZE=1"
        "-sEXPORT_NAME=\"createModule\""
        "-sEXPORTED_RUNTIME_METHODS=[\"callMain\",\"addOnExit\",\"JSEvents\"]"
    )

    target_link_options(oci PRIVATE "--embed-file=${CMAKE_CURRENT_SOURCE_DIR}/res/@/res/")
    target_link_options(oci PRIVATE "--embed-file=${CMAKE_CURRENT_BINARY_DIR}/ChickenInvaders2.dat@/ChickenInvaders2.dat")
else()
    if (${USE_SFML})
        # https://github.com/SFML/cmake-sfml-project/blob/master/CMakeLists.txt
        FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.x
            GIT_SHALLOW ON
            SYSTEM)
        FetchContent_MakeAvailable(SFML)
        target_link_libraries(oci PRIVATE sfml-graphics sfml-audio)
        target_include_directories(oci PRIVATE ${sfml_SOURCE_DIR}/include)
        target_compile_definitions(oci PRIVATE "USE_SFML")
    else ()
        find_package(SDL2 CONFIG REQUIRED)
        target_link_libraries(oci PRIVATE SDL2::SDL2)

        find_package(SDL2_mixer QUIET)
        if (SDL2_mixer_FOUND)
            target_link_libraries(oci PRIVATE SDL2_mixer::SDL2_mixer)
        else ()
            # cmake can't find the system SDL2-mixer package in Ubuntu 22.04
            # try to use pkgconfig in this case
            find_package(PkgConfig REQUIRED)
            pkg_check_modules(SDL2_mixer REQUIRED SDL2_mixer)
            target_link_libraries(oci PRIVATE ${SDL2_mixer_LIBRARIES})
            target_include_directories(oci PRIVATE ${SDL2_mixer_INCLUDE_DIRS})
            target_compile_options(oci PRIVATE ${SDL2_mixer_CFLAGS_OTHER})
        endif()
    endif()

    install(TARGETS oci RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ChickenInvaders2.dat
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/res/
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    add_custom_command(
        TARGET oci
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/res/
            $<TARGET_FILE_DIR:oci>/res/)

    if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        set(RUNTIME_DLL_SET $<BOOL:$<TARGET_RUNTIME_DLLS:oci>>)
        set(COPY_COMMAND
            ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:oci>
            $<TARGET_FILE_DIR:oci>
        )
        add_custom_command(
            TARGET oci
            POST_BUILD
            COMMAND "$<${RUNTIME_DLL_SET}:${COPY_COMMAND}>"
            COMMAND_EXPAND_LISTS
            COMMENT "Copying runtime dependencies (DLLs)"
        )
    endif()
endif()

if (MSVC)
    target_compile_options(oci PRIVATE "/D_CRT_SECURE_NO_WARNINGS" "/GR-" "/EHsc" "/D_HAS_EXCEPTIONS=0")
else()
    target_compile_options(oci PRIVATE "-fno-rtti" "-fno-exceptions" "-Wextra" "-Wall")
endif()

target_include_directories(oci PRIVATE ${CMAKE_SOURCE_DIR}/src)

if (${USE_IWYU})
    find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
    if (NOT iwyu_path)
        message("WARNING: include-what-you-use tool was not found - will build without include validation")
    else()
        set_property(TARGET oci PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path};-Xiwyu;--cxx17ns)
    endif()
endif ()
