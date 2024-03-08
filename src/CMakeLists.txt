file (GLOB_RECURSE MY_SOURCE_FILES CONFIGURE_DEPENDS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)
target_sources (${CMAKE_PROJECT_NAME} PRIVATE ${MY_SOURCE_FILES} "../include/OurMath.h" "../include/Menu.h" "../include/Play.h" "Menu.cpp" "../include/Button.h" "Button.cpp" "../include/Exit.h" "../include/Help.h"  "Exit.cpp" "Help.cpp" "Play.cpp")
