from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

'''
building:
    $ conan install . [-pr:b=default]
    $ cmake --preset release
    $ cmake --build --preset release
'''

class EDAFinal(ConanFile):
    name = "EDAFinal"
    version = "0.1"

    # requires = "gmp/6.2.1", "flint/2.9.0"
    requires = "flint/2.9.0"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    generators = "CMakeDeps", "CMakeToolchain"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    # def generate(self):
    #     tc = CMakeToolchain(self)
    #     tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    # def package_info(self):
    #     # self.cpp_info.libs = ["hello"]
    #     pass

