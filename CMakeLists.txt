cmake_minimum_required(VERSION 2.8)

project("spi")

add_definitions(-std=c++11)

add_executable(spi 
  main.cc 
  parser.cc
)
