from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class featherRecipe(ConanFile):
    name = "feather"
    version = "0.1.0"
    package_type = "library"

    # Optional metadata
    license = "MIT"
    author = "notforest sshkliaiev@gmail.com"
    url = "https://github.com/not-forest/feather-engine"
    description = "A lightweight game/simulation engine for 2D/3D rendering, input, and audio."
    topics = ("engine", "sdl2", "opengl", "game", "visual", "physics")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"
    def requirements(self):
        self.requires("tllist/1.1.0")
        self.requires("sdl/2.30.8")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["feather"]
        self.cpp_info.includedirs = ["include"]
