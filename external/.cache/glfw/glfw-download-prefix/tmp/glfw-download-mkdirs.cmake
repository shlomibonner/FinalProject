# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Dev/FinalProject/CG3DBasicEngine-main/cmake/../external/glfw"
  "C:/Dev/FinalProject/FinalProjectCompiled/glfw-build"
  "C:/Dev/FinalProject/CG3DBasicEngine-main/external/.cache/glfw/glfw-download-prefix"
  "C:/Dev/FinalProject/CG3DBasicEngine-main/external/.cache/glfw/glfw-download-prefix/tmp"
  "C:/Dev/FinalProject/CG3DBasicEngine-main/external/.cache/glfw/glfw-download-prefix/src/glfw-download-stamp"
  "C:/Dev/FinalProject/CG3DBasicEngine-main/external/.cache/glfw/glfw-download-prefix/src"
  "C:/Dev/FinalProject/CG3DBasicEngine-main/external/.cache/glfw/glfw-download-prefix/src/glfw-download-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Dev/FinalProject/CG3DBasicEngine-main/external/.cache/glfw/glfw-download-prefix/src/glfw-download-stamp/${subDir}")
endforeach()
