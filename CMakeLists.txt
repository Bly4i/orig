   cmake_minimum_required(VERSION 3.15)
   project(MyProject)

   set(CMAKE_CXX_STANDARD 20) 

   include_directories("C:/Users/user/vcpkg/installed/x64-windows/include")
   link_directories("C:/Users/user/vcpkg/installed/x64-windows/lib")

   add_executable(MyProject src/main.cpp)

   target_link_libraries(MyProject 
       pqxx
       libpq 
   )
