add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"}) -- generate compile commands

set_warnings("everything")

if is_plat("linux") then
	set_policy("build.sanitizer.address", true)
	set_policy("build.sanitizer.leak", true)
end

add_languages("c99")

target("fir")
	add_rules("utils.install.pkgconfig_importfiles")
	set_version("1.0.0")

	set_kind("static")

	add_files("src/*.c")

	add_includedirs("include", {public=true})
	add_headerfiles("include/(fir/*.h)", {public=true})
target_end()
