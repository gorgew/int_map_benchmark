add_requires("vcpkg::nanobench")
target("benchmark")
do
	set_kind("binary")
	add_files("src/*.cpp")
	add_packages("vcpkg::nanobench")
end
