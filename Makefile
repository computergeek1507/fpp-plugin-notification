include /opt/fpp/src/makefiles/common/setup.mk

all: libfpp-plugin-notification.so
debug: all

OBJECTS_fpp_notification_so += src/NotificationPlugin.o src/notificationBase.o src/pushover.o src/email.o src/telegram.o
LIBS_fpp_notification_so += -L/opt/fpp/src -lfpp
CXXFLAGS_src/openHABPlugin.o += -I/opt/fpp/src

%.o: %.cpp Makefile
	$(CCACHE) $(CC) $(CFLAGS) $(CXXFLAGS) $(CXXFLAGS_$@) -c $< -o $@

libfpp-plugin-notification.so: $(OBJECTS_fpp_notification_so) /opt/fpp/src/libfpp.so
	$(CCACHE) $(CC) -shared $(CFLAGS_$@) $(OBJECTS_fpp_notification_so) $(LIBS_fpp_notification_so) $(LDFLAGS) -o $@

clean:
	rm -f libfpp-plugin-notification.so $(OBJECTS_fpp_notification_so)
