CXXFLAGS := -std=c++17

debug: debug.cc

.PHONY: clean
clean:
	rm -f debug
