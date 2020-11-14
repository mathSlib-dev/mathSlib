cmake_minimum_required(VERSION 3.13)
project(mathS)

set(SHARED OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(
        mathS/include
)
AUX_SOURCE_DIRECTORY(mathS/src DIR_SRCS)

add_executable(mathS mathS/include/LBAssembler.h mathS/include/MathObject.h mathS/include/MathParser.h
        mathS/include/NMathObject.h
        mathS/include/Rule.h
        mathS/include/RuleLib.h
        mathS/include/NFunction.h
        mathS/include/NFunctionalOperator.h
        mathS/include/ExtendedCMath.h
        mathS/include/Compute.h
        mathS/src/LBAssembler.cpp mathS/src/MathObject.cpp mathS/src/MathParser.cpp
        mathS/src/NMathObject.cpp
        mathS/src/NFunction.cpp
        mathS/src/ExtendedCMath.cpp
        mathS/src/Rule.cpp
        mathS/src/RuleLib.cpp
        mathS/src/Compute.cpp
        mathS/src/NFunctionalOperator.cpp
        mathS/src/main.cpp mathS/src/lexer.cpp mathS/include/lexer.h)