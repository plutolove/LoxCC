add_rules("mode.debug", "mode.release")

set_languages("c++17")
set_optimize("fastest")
-- set_optimize("none")

add_rules("plugin.compile_commands.autoupdate", { outputdir = "build" })

add_repositories("local-repo https://github.com/plutolove/xmake-repo.git")

add_cxxflags("-fuse-ld=lld")

add_requires("libllvm 18.x", { configs = { shared = true } })

add_requires("fmt 10.0.0")
add_requires("spdlog v1.11.0")
add_requires("boost 1.84.0")
add_requires("magic_enum v0.9.5")
add_requires("gflags v2.2.2")

rule("dep_gen")
  before_build(function(target)
    os.vrun("mlir-tblgen  --gen-dialect-decls ./src/ir/lox.td -o ./src/ir/gen/Dialect.h.inc -I./src/ -I"..os.getenv("LLVM_ROOT").."/include/")
	  os.vrun("mlir-tblgen  --gen-dialect-defs ./src/ir/lox.td -o ./src/ir/gen/Dialect.cpp.inc -I./src/ -I"..os.getenv("LLVM_ROOT").."/include/")

	  os.vrun("mlir-tblgen  --gen-op-decls ./src/ir/lox.td -o ./src/ir/gen/loxOps.h.inc -I./src/ -I"..os.getenv("LLVM_ROOT").."/include/")
	  os.vrun("mlir-tblgen  --gen-op-defs ./src/ir/lox.td -o ./src/ir/gen/loxOps.cpp.inc -I./src/ -I"..os.getenv("LLVM_ROOT").."/include/")

    os.vrun("mlir-tblgen  --gen-typedef-decls ./src/ir/lox.td -o ./src/ir/gen/loxTypes.h.inc -I./src/ -I"..os.getenv("LLVM_ROOT").."/include/")
	  os.vrun("mlir-tblgen  --gen-typedef-defs ./src/ir/lox.td -o ./src/ir/gen/loxTypes.cpp.inc -I./src/ -I"..os.getenv("LLVM_ROOT").."/include/")

end)

target("Lox")
    add_rules("dep_gen")
    set_kind("binary")
    set_symbols("debug")
    set_strip("all")
    add_includedirs("./src")
    add_files("src/*/*.cpp")
    add_files("src/*/*/*.cpp")
    add_files("src/*.cpp")
    add_packages("fmt", "abseil", "spdlog", "boost", "magic_enum", "libllvm", "gflags")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

