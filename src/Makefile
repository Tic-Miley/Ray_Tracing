CXX = g++
CXXFLAGS = -std=c++17 

TARGET = RayTracer
SRC = main.cpp Trace.cpp Scene.cpp BlinnPhong.cpp BVH.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) -O1 $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	del $(TARGET).exe

run: $(TARGET)
	.\$(TARGET) $(args)