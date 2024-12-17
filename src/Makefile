CXX = g++
CXXFLAGS = -std=c++17 

TARGET = RayTracer
SRC = main.cpp Trace.cpp Scene.cpp

all: $(TARGET)

	# $(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
$(TARGET): $(SRC)
	$(CXX) -O3 -Wl,--stack,1073741824 $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	del $(TARGET).exe

run: $(TARGET)
	.\$(TARGET) $(args)