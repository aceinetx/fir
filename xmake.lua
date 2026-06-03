add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"}) -- generate compile commands

set_warnings("everything")

if is_plat("linux") then
	set_policy("build.sanitizer.address", true)
	set_policy("build.sanitizer.leak", true)
end

add_languages("c99", "c++11")

target("fir")
	add_rules("utils.install.pkgconfig_importfiles")
	set_version("1.0.0")

	set_kind("static")

	add_files("src/*.c")

	add_includedirs("include", {public=true})
	add_headerfiles("include/(fir/*.h)", {public=true})
target_end()

target("fircpp")
	add_rules("utils.install.pkgconfig_importfiles")
	set_version("1.0.0")

	set_kind("static")

	add_files("fircpp/src/*.cpp")

	add_includedirs("fircpp/include", {public=true})
	add_headerfiles("fircpp/include/(fircpp/*.h)", {public=true})

	add_deps("fir")
target_end()

target("fircpp_test")
	set_kind("binary")

	add_files("fircpp/test.cpp")

	add_deps("fircpp")
target_end()
