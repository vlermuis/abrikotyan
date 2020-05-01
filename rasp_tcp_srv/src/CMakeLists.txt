

if(TEST_CLIENT)
add_executable(test_client test_client.c)
endif()

#add_executable(rasp_tcp_srv rasp_srv.c motor_ctrl.c)
if (HAS_RASP_SRV_ONLY)
add_executable(raspis main.c rasp_srv.c )
else()
    add_executable(raspis main.c rasp_srv.c motor_ctrl.c)
    target_link_libraries(raspis wiringPi)
    target_link_libraries(raspis crypt)
    target_link_libraries(raspis pthread)
    target_link_libraries(raspis rt)
    target_link_libraries(raspis m)
endif()





