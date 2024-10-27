import os

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run


class featherTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("tllist/1.1.0")
        self.requires("sdl/2.30.8")
        self.requires(self.tested_reference_str)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            tests = "minimal", "layer_counter"
            for test in tests:
                cmd = os.path.join(self.cpp.build.bindir, test)
                self.run(cmd, env="conanrun", ignore_errors=True)
