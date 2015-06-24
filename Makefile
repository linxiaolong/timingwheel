.PHONY: clean

CFLAGS = -g3 -O2 -rdynamic -Wall -I$(INCLUDE_DIR) 
SHARED = -fPIC --shared
LDFLAGS = -L$(BUILD_DIR) -Wl,-rpath $(BUILD_DIR)

test: ./twheel.c ./test.c
	gcc $^ -o $@ -pthread

clean:
	-rm -rf test *.o *.a
