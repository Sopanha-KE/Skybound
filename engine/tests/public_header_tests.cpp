#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

// Enforces the architecture rule that public headers never expose third-party types.
TEST_CASE("Public headers include no third-party libraries", "[architecture]") {
    const std::vector<std::string> forbidden = {"SDL", "spdlog", "fmt/", "glad", "entt", "Jolt",
                                                "assimp", "fastgltf", "imgui"};
    int checked = 0;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(SKY_PUBLIC_HEADER_DIR)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".hpp") continue;
        ++checked;
        std::ifstream file(entry.path());
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("#include") == std::string::npos) continue;
            INFO(entry.path().string() << ": " << line);
            for (const auto& bad : forbidden) CHECK(line.find(bad) == std::string::npos);
        }
    }
    REQUIRE(checked > 0);
}
