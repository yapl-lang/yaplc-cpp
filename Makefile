LOCAL_PATH := $(shell pwd)
include $(LOCAL_PATH)/project/yapl.mk

CC=$(APP_CPP_COMPILER)
CFLAGS=$(APP_CPPFLAGS) -c -Wall -I$(LOCAL_C_INCLUDES)
LDFLAGS=$(LOCAL_LDLIBS)
SOURCES=$(abspath $(foreach dir,$(LOCAL_SRC_FILES),$(abspath $(dir))))
OBJECTS=$(SOURCES:.c=.o)
OBJECTS:=$(OBJECTS:.cpp=.o)
OBJECTS:=$(subst $(abspath $(LOCAL_PATH)/src),$(abspath $(LOCAL_PATH)/obj),$(OBJECTS))
EXECUTABLE=bin/$(LOCAL_MODULE)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir bin
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

#.cpp.o:
$(LOCAL_PATH)/obj/%.o: $(LOCAL_PATH)/src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(LOCAL_PATH)/obj/%.o: $(LOCAL_PATH)/src/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -r $(OBJECTS)

obj: $(OBJECTS)