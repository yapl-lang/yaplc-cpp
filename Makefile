LOCAL_PATH := $(shell pwd)
include $(LOCAL_PATH)/project/yapl.mk

CC=$(APP_CPP_COMPILER)
CFLAGS=$(APP_CPPFLAGS) -c -Wall -I$(LOCAL_C_INCLUDES)
LDFLAGS=$(LOCAL_LDLIBS)
SOURCES=$(realpath $(foreach dir,$(LOCAL_SRC_FILES),$(shell realpath $(LOCAL_PATH)/src/$(dir))))
OBJECTS=$(SOURCES:.c=.o)
OBJECTS:=$(OBJECTS:.cpp=.o)
EXECUTABLE=$(LOCAL_MODULE)
$(info ${OBJECTS})

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


clear:
	rm -r $(OBJECTS)