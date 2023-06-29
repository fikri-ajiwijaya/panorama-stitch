CFLAGS := -std=c17
CXXFLAGS := -std=c++17

all: debug equidist-equirect

debug: debug.cc

equidist-equirect: equidist-equirect.cc stb_image.o stb_image_write.o

.PHONY: clean
clean:
	rm -f debug equidist-equirect
	rm -f *.o
