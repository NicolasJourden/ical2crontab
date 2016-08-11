
#add_executable(getGoogle google.c)
#target_link_libraries(getGoogle ${CURL_LIBRARIES})
 
# libical to parse the calendar:
add_executable(ical2crontab main.c ical.c ical_dump.c)
target_link_libraries(ical2crontab ${LIBICAL_LIBRARIES})

