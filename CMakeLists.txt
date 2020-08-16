cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(vitaUsbty)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -fshort-wchar -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
	${CMAKE_SOURCE_DIR}
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(-DFW360CEX)

if (NOT ${RELEASE})
  add_definitions(-DENABLE_LOGGING)
endif()

# Builds
function(ADD_RESOURCES out_var)
  set(result)
  foreach(in_f ${ARGN})
    set(out_f "${CMAKE_CURRENT_BINARY_DIR}/${in_f}.o")
    get_filename_component(out_dir ${out_f} DIRECTORY)
    add_custom_command(OUTPUT ${out_f}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${out_dir}
      COMMAND ${CMAKE_LINKER} -r -b binary -o ${out_f} ${in_f}
      DEPENDS ${in_f}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Building resource ${out_f}"
      VERBATIM
      )
    list(APPEND result ${out_f})
  endforeach()
  set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

file(GLOB res_files RELATIVE ${CMAKE_SOURCE_DIR} *.xml)
add_resources(xml_res ${res_files})

#link_directories(${CMAKE_SOURCE_DIR}/vconsole-stubs)


add_executable(kernel_usb
  kmain.c
  cmd.c
  kfuncs.c
  serial.c
  utils.c
  hooks.c
  syscon_trace.c
)

target_link_libraries(kernel_usb
  gcc
  SceSysmemForDriver_stub
  SceModulemgrForKernel_stub
  SceModulemgrForDriver_stub
  taihenForKernel_stub
  SceDebugForKernel_stub
  SceDebugForDriver_stub
  SceThreadmgrForDriver_stub
  taihenModuleUtils_stub
  SceSysclibForDriver_stub
  SceSysconForDriver_stub
  ScePowerForDriver_stub
)


set_target_properties(kernel_usb
  PROPERTIES LINK_FLAGS "-nostdlib"
  COMPILE_FLAGS "-D__VITA_KERNEL__"
)

vita_create_self(vitaUsbtty.skprx kernel_usb
  CONFIG ${CMAKE_SOURCE_DIR}/exports_kernel_usb.yml
  UNSAFE
)
