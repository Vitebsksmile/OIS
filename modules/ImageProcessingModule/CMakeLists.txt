add_library(ImageProcessingModule STATIC


#    filters.cpp
#    filters.h
#    calibration.cpp
#    calibration.h
)

target_link_libraries(ImageProcessingModule PRIVATE
    Qt6::Core
    Qt6::Gui
    ${OpenCV_LIBS}
)

target_include_directories(ImageProcessingModule PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
