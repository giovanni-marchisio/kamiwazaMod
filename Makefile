ifeq ($(OS),Windows_NT)
	CXX		:= g++
	MKDIR	:= mkdir
	RM		:= del /Q
else
	CXX		:= x86_64-w64-mingw32-g++
	MKDIR	:= mkdir -p
	RM		:= rm -f
endif 

CXXFLAGS	:= -shared -O2 -Wall
LDFLAGS		:= -static

SRC			:= ./src/dll/*.cpp ./src/memory/*.cpp
DEF			:= ./dwmapi.def
BUILD		:= ./build/
TARGET		:= ./build/dwmapi.dll

.PHONY: all clean

all: $(TARGET)

$(TARGET):	$(SRC) $(DEF)
	$(MKDIR) $(BUILD)
	$(CXX) $(CXXFLAGS) $(SRC) $(DEF) -o $(TARGET) $(LDFLAGS)

clean:
	$(RM) $(TARGET)