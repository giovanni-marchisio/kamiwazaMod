ifeq ($(OS),Windows_NT)
	CXX		:= g++
	MKDIR	:= mkdir
	RM		:= del /Q
else
	CXX		:= x86_64-w64-mingw32-g++
	MKDIR	:= mkdir -p
	RM		:= rm -f
endif 

CXXFLAGS	:= -shared -O2 -Wall -I./src/MinHook/include/ -I./src/MinHook/include/hde/ -I./src/hook/
LDFLAGS		:= -static
LDLIBS		:= ./src/MinHook/minhook.a -ld3d11 -ldxgi

SRC			:= ./src/dll/*.cpp ./src/dummy/*.cpp ./src/memory/*.cpp ./src/hook/*.cpp
DEF			:= ./dwmapi.def
BUILD		:= ./build/
TARGET		:= ./build/dwmapi.dll

.PHONY: all clean

all: $(TARGET)

$(TARGET):	$(SRC) $(DEF)
	$(MKDIR) $(BUILD)
	$(CXX) $(CXXFLAGS) $(SRC) $(DEF) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(TARGET)