
set(HEADER_LIST "${Mr_Text_SOURCE_DIR}/include/MrText/MrText.hpp"
		"${Mr_Text_SOURCE_DIR}/include/MrText/ArrayBuffer.hpp"
		"${Mr_Text_SOURCE_DIR}/include/MrText/KeyMap.hpp"
		"${Mr_Text_SOURCE_DIR}/include/MrText/Rope.hpp"
		"${Mr_Text_SOURCE_DIR}/include/MrText/Node.hpp"
		"${Mr_Text_SOURCE_DIR}/include/MrText/TextInfo.hpp")

set(FILE_LIST "MrText.cpp" "ArrayBuffer.cpp" "KeyMap.cpp"
		"Rope.cpp" "Node.cpp" "TextInfo.cpp")
add_library(MrText ${FILE_LIST} ${HEADER_LIST})

target_include_directories(MrText PUBLIC ../include)

set_target_properties(MrText PROPERTIES
    CXX_CLANG_TIDY_EXPORT_FIXES_DIR "${CMAKE_BINARY_DIR}/build/clang-tidy-fixes"
)

target_link_libraries(MrText PRIVATE)

target_compile_features(MrText PUBLIC)


