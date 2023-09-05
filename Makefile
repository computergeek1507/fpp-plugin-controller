include /opt/fpp/src/makefiles/common/setup.mk

all: libfpp-plugin-controller.so
debug: all


OBJECTS_fpp_controller_so += src/controllerPlugin.o src/controller_base.o src/falconV4_controller.o src/falconV3_controller.o src/genius_controller.o  src/fpp_controller.o
LIBS_fpp_controller_so += -L/opt/fpp/src -lfpp
CXXFLAGS_src/controllerPlugin.o += -I/opt/fpp/src

%.o: %.cpp Makefile
	$(CCACHE) $(CC) $(CFLAGS) $(CXXFLAGS) $(CXXFLAGS_$@) -c $< -o $@

libfpp-plugin-controller.so: $(OBJECTS_fpp_controller_so) /opt/fpp/src/libfpp.so
	$(CCACHE) $(CC) -shared $(CFLAGS_$@) $(OBJECTS_fpp_controller_so) $(LIBS_fpp_controller_so) $(LDFLAGS) -o $@

clean:
	rm -f libfpp-plugin-controller.so $(OBJECTS_fpp_controller_so)
