ifeq ($(OS),Windows_NT)
	CXX		:= g++
	MKDIR	:= mkdir
	RM		:= del /Q
else
	CXX		:= x86_64-w64-mingw32-g++
	MKDIR	:= mkdir -p
	RM		:= rm -f
endif 

CXXFLAGS	:= -shared -O2 -Wall -I./minhook/include/ -I./minhook/include/hde/ -I./imgui/ -I./src/hook/
LDFLAGS		:= -static
LDLIBS		:= ./minhook/minhook.a -ld3d11 -ldxgi -ld3dcompiler -lgdi32 -luser32 -ldwmapi

SRC			:= ./src/dll/*.cpp ./src/memory/*.cpp ./src/dummy/*.cpp ./src/hook/*.cpp ./src/ui/*.cpp ./imgui/*.cpp ./imgui/backends/*.cpp
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