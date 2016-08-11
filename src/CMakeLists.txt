# Creating ical2crontab bin file:
add_executable(ical2crontab main.c ical.c ical_dump.c)
target_link_libraries(ical2crontab ${LIBICAL_LIBRARIES})
install (TARGETS ical2crontab DESTINATION bin)
