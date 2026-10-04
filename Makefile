CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -IFisiere_HPP

SRC_DIR := Fisiere_CPP
TARGET  := GonjiamGame.exe

OBJS := Types.o Tool.o Tools.o Player.o Event.o Location.o \
        NarrationLine.o StoryManager.o Listener.o Renderer.o GameEngine.o main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

Types.o: $(SRC_DIR)/Types.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Types.cpp -o Types.o

Tool.o: $(SRC_DIR)/Tool.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Tool.cpp -o Tool.o

Tools.o: $(SRC_DIR)/Tools.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Tools.cpp -o Tools.o

Player.o: $(SRC_DIR)/Player.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Player.cpp -o Player.o

Event.o: $(SRC_DIR)/Event.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Event.cpp -o Event.o

Location.o: $(SRC_DIR)/Location.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Location.cpp -o Location.o

NarrationLine.o: $(SRC_DIR)/NarrationLine.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/NarrationLine.cpp -o NarrationLine.o

StoryManager.o: $(SRC_DIR)/StoryManager.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/StoryManager.cpp -o StoryManager.o

Listener.o: $(SRC_DIR)/Listener.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Listener.cpp -o Listener.o

Renderer.o: $(SRC_DIR)/Renderer.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Renderer.cpp -o Renderer.o

GameEngine.o: $(SRC_DIR)/GameEngine.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/GameEngine.cpp -o GameEngine.o

main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o main.o

clean:
	rm -f *.o *.exe