CXX := g++
AR := ar
CXXFLAGS := -std=c++17 -Wall -Wextra -IFisiere_HPP $(shell pkg-config --cflags sqlite3)
SQLITE_LIBS := $(shell pkg-config --libs sqlite3)

SRC_DIR := Fisiere_CPP
HDRS := $(wildcard Fisiere_HPP/*.hpp)
TARGET := GonjiamGame.exe
LIB_DIR := build/lib

APP_OBJS := StoryManager.o Listener.o Renderer.o ConsoleRenderer.o GameEngine.o main.o
DATA_LIBS := $(LIB_DIR)/libtypes.a $(LIB_DIR)/libtool.a $(LIB_DIR)/libtools.a \
             $(LIB_DIR)/libplayer.a $(LIB_DIR)/libevent.a $(LIB_DIR)/liblocation.a \
             $(LIB_DIR)/libnarrationline.a

all: $(TARGET)

$(TARGET): $(APP_OBJS) $(DATA_LIBS)
	$(CXX) -o $@ $(APP_OBJS) -Wl,--start-group $(DATA_LIBS) -Wl,--end-group $(SQLITE_LIBS)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(LIB_DIR)/libtypes.a: Types.o | $(LIB_DIR)
	$(AR) rcs $@ $^

$(LIB_DIR)/libtool.a: Tool.o | $(LIB_DIR)
	$(AR) rcs $@ $^

$(LIB_DIR)/libtools.a: Tools.o | $(LIB_DIR)
	$(AR) rcs $@ $^

$(LIB_DIR)/libplayer.a: Player.o | $(LIB_DIR)
	$(AR) rcs $@ $^

$(LIB_DIR)/libevent.a: Event.o | $(LIB_DIR)
	$(AR) rcs $@ $^

$(LIB_DIR)/liblocation.a: Location.o | $(LIB_DIR)
	$(AR) rcs $@ $^

$(LIB_DIR)/libnarrationline.a: NarrationLine.o | $(LIB_DIR)
	$(AR) rcs $@ $^

Types.o: $(SRC_DIR)/Types.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

Tool.o: $(SRC_DIR)/Tool.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

Tools.o: $(SRC_DIR)/Tools.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

Player.o: $(SRC_DIR)/Player.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

Event.o: $(SRC_DIR)/Event.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

Location.o: $(SRC_DIR)/Location.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

NarrationLine.o: $(SRC_DIR)/NarrationLine.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

StoryManager.o: $(SRC_DIR)/StoryManager.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

Listener.o: $(SRC_DIR)/Listener.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

Renderer.o: $(SRC_DIR)/Renderer.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

ConsoleRenderer.o: $(SRC_DIR)/Consolerenderer.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

GameEngine.o: $(SRC_DIR)/GameEngine.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: $(SRC_DIR)/Main.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o *.exe
	rm -rf $(LIB_DIR)

.PHONY: all clean
