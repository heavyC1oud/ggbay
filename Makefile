


all: ggbay

ggbay: main.cpp
	$(CXX) $(CXXFLAGS) -o ggbay main.cpp


.PHONY: clean

clean:
	rm -f *.o
	rm -f ggbay