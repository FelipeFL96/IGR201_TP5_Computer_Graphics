set(GLM_VERSION "0.9.9")
set(GLM_INCLUDE_DIRS "/cal/homes/fdelima/IGR201/computer_graphics/BaseGL/External/glm")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
