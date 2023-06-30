LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

CFLAGS := -std=c17
CXXFLAGS := -std=c++17

all: debug equidist-equirect equirect-mask equirect-blend

debug: debug.o _image.o
debug.o: debug.cc image.hh makefile

equidist-equirect: equidist-equirect.o angle.o _image.o
equidist-equirect.o: equidist-equirect.cc angle.hh image.hh constant.hh makefile

equirect-mask: equirect-mask.o angle.o _image.o
equirect-mask.o: equirect-mask.cc angle.hh image.hh constant.hh makefile

equirect-blend: equirect-blend.o _image.o
equirect-blend.o: equirect-blend.cc image.hh makefile

angle.o: angle.cc angle.hh constant.hh makefile

_image.o: image.o stb_image.o stb_image_write.o
	$(LD) -relocatable $^ -o $@
image.o: image.cc image.hh makefile

stb_image.o: stb_image.c makefile
stb_image_write.o: stb_image_write.c makefile

.PHONY: clean
clean:
	rm -f debug equidist-equirect equirect-mask
	rm -f *.o
