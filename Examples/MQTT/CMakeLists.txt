project(MQTTSubscriber)

add_executable(MQTTSubscriber main.cpp)

find_library(MOSQUITTO_LIBRARY mosquitto)

target_link_libraries(MQTTSubscriber ${MOSQUITTO_LIBRARY})

set_property(TARGET MQTTSubscriber PROPERTY CXX_STANDARD 17)
