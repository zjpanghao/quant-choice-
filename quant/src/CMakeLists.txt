set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../bin/)
SET(CMAKE_CXX_COMPILER "g++")
SET(PRO_DIR "/home/panghao/git/quant-choices3")
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
${KUNLIB}/include/zookeeper 
${KUNLIB}/include )
link_directories(${PRO_DIR}/quant/bin 
${KUNLIB}/libs/zookeeper 
${KUNLIB}/libs)
add_executable(quant3 cst_handle.cpp kafka_producer.cpp syn_handle.cpp asyn_handle.cpp quant_code.cpp css_handle.cpp csq_handle.cpp quant_pack.cpp quant_task.cpp quant_util.cpp indictor_info.cpp  main.cpp stock_info.cpp)
target_link_libraries(quant3 quant hiredis zookeeper_mt rdkafka glog dl)
