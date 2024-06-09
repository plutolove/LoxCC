add_rules("mode.debug", "mode.release")

set_languages("c++17")
-- set_optimize("fastest")
set_optimize("none")
add_cxxflags("-fuse-ld=lld")

add_rules("plugin.compile_commands.autoupdate", { outputdir = "build" })

add_repositories("local-repo https://github.com/plutolove/xmake-repo.git")


add_requires("libllvm 15.x", { configs = { shared = true } })
add_requires("fmt 10.0.0")
add_requires("spdlog v1.11.0")
add_requires("boost 1.84.0")
add_requires("magic_enum v0.9.5")
add_requires("gflags v2.2.2")

target("Lox")
    set_kind("binary")
    set_symbols("debug")
    set_strip("all")
    add_includedirs("./src")
    add_files("src/*/*.cpp")
    add_files("src/*/*/*.cpp")
    add_files("src/*.cpp")
    add_packages("fmt", "abseil", "spdlog", "boost", "magic_enum", "libllvm", "gflags")

