CXXFLAGS := -std=c++17

debug: debug.cc stb_image.c stb_image_write.c

.PHONY: clean
clean:
	rm -f debug
