project(
  UGridNET
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "UGrid C# API"
  LANGUAGES CSharp
)

include(CSharpUtilities)

# target name
set(TARGET_NAME ${PROJECT_NAME})

set(ASSEMBLY_INFO_CS ${CMAKE_CURRENT_BINARY_DIR}/Properties/AssemblyInfo.cs)
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/../cs_proj_templates/AssemblyInfo.cs.in
  ${ASSEMBLY_INFO_CS}
)

set(APP_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/App.config)
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/../cs_proj_templates/App.config.in
  ${APP_CONFIG}
)

add_library(
  ${TARGET_NAME}
  SHARED 
  ${ASSEMBLY_INFO_CS}
  ${APP_CONFIG}
  ${SWIG_GENERATED_CSHARP_SRCS}
)

set_property(TARGET ${TARGET_NAME} PROPERTY VS_DOTNET_REFERENCES "System")

#target_compile_options(${TARGET_NAME} PRIVATE "/platform:${GLOB_DOTNET_TARGET_PLATFORM}")
#target_compile_options(${TARGET_NAME} PRIVATE "/langversion:${GLOB_CSHARP_LANGUAGE_VERSION}")

#Copy UGridCSWrapper
add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:UGridCSWrapper>" "$<TARGET_FILE_DIR:UGridNET>"
    COMMENT "Copy $<TARGET_FILE:UGridCSWrapper> to $<TARGET_FILE_DIR:UGridNET>"
)

add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
    COMMAND echo "[[[[ ${TARGET_NAME}  : DONE ]]]"
)