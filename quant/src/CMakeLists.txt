set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../bin/)
SET(CMAKE_CXX_COMPILER "g++")
SET(PRO_DIR "/home/panghao/git/quant-choice/quant-choices")
add_definitions(-std=c++11)
add_definitions(-DUSER_DELTA=1)
add_definitions(-g)
set(HH ${PRO_DIR}/quant/EmQuantAPI)
message(${HH})
set(KUNLIB /home/panghao/kunlib)
include_directories(
${PRO_DIR}/quant/EmQuantAPI 
${KUNLIB}/include/librdkafka 
${KUNLIB}/include/hiredis 
${KUNLIB}/include/zdb 
${KUNLIB}/include/zookeeper 
${KUNLIB}/include/election 
${KUNLIB}/include 
${KUNLIB}/user)
link_directories(${PRO_DIR}/quant/bin 
${KUNLIB}/libs/zookeeper 
${KUNLIB}/libs)
#include_directories(${PRO_DIR}/quant/EmQuantAPI  /home/panghao/include/zookeeper /home/panghao/include /home/panghao/include/hiredis /home/panghao/librdkafka /usr/local/include/librdkafka /usr/local/include/zdb)
#link_directories(${PRO_DIR}/quant/bin /home/panghao/libs/zookeeper /home/panghao/libs)
add_executable(quant3_2 quant_process.cpp day_line.cpp cst_handle.cpp kafka_producer.cpp syn_handle.cpp asyn_handle.cpp quant_code.cpp css_handle.cpp csq_handle.cpp quant_pack.cpp quant_task.cpp quant_util.cpp indictor_info.cpp  main.cpp stock_info.cpp)
target_link_libraries(quant3_2 kunconfig quant hiredis pacsdbpool zookeeper_mt rdkafka glog dl)
