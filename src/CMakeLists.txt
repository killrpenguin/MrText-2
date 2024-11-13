set(HEADER_LIST "${Mr_Text_SOURCE_DIR}/include/MrText/MrText.hpp" "${Mr_Text_SOURCE_DIR}/include/MrText/ArrayBuffer.hpp" "${Mr_Text_SOURCE_DIR}/include/MrText/KeyMap.hpp")

add_library(MrText MrText.cpp ArrayBuffer.cpp KeyMap.cpp ${HEADER_LIST})

target_include_directories(MrText PUBLIC ../include)

target_link_libraries(MrText PRIVATE)

target_compile_features(MrText PUBLIC)


