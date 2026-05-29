
CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Isrc -Isrc/desktop -Isrc/shell
LDFLAGS ?=
LDLIBS ?=

X11_CFLAGS := $(shell pkg-config --cflags x11 2>/dev/null)
X11_LIBS := $(shell pkg-config --libs x11 2>/dev/null)

CPPFLAGS += $(X11_CFLAGS)
LDLIBS += $(X11_LIBS)

ifeq ($(strip $(LDLIBS)),)
LDLIBS += -lX11
endif

BUILD_DIR := build
DIST_DIR := $(BUILD_DIR)/dist
TARGET := $(BUILD_DIR)/horizon-desktop
SRC_FILES := \
	main.c \
	desktop/HorizonDesktop.c \
	desktop/HorizonWallpapers.c \
	desktop/HorizonDesktopIcons.c \
	desktop/XorgMouse.c \
	shell/HorizonCompositor.c \
	shell/HorizonWindow.c \
	shell/HorizonTaskbar.c \
	shell/HorizonStartmenu.c \
	shell/HorizonFontHandler.c

SOURCES := $(addprefix src/,$(SRC_FILES))
OBJECTS := $(addprefix $(DIST_DIR)/src/,$(SRC_FILES:.c=.o))

.PHONY: all clean run

all: $(TARGET)

$(BUILD_DIR) $(DIST_DIR):
	mkdir -p $@

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
	rm -f horizon-desktop
	rm -f src/*.o src/desktop/*.o src/shell/*.o

$(DIST_DIR)/src/%.o: src/%.c | $(DIST_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
