
add_executable(test_client test_client.c)
add_executable(rasp_tcp_srv rasp_srv.c motor_ctrl.c)
target_link_libraries(rasp_tcp_srv wiringPi)
target_link_libraries(rasp_tcp_srv crypt)
target_link_libraries(rasp_tcp_srv pthread)
target_link_libraries(rasp_tcp_srv rt) 
target_link_libraries(rasp_tcp_srv m)

