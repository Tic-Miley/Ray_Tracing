CXX = g++
CXXFLAGS = -std=c++17 

TARGET = RayTracer
SRC = main.cpp Trace.cpp Scene.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	del $(TARGET).exe

run: $(TARGET)
	./$(TARGET)