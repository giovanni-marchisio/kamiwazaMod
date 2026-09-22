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
SRC			:= ./src/dll/dllmain.cpp
BUILD		:= ./build/
TARGET	:= ./build/dwmapi.dll

.PHONY: all clean

all:	$(TARGET)

$(TARGET):	$(SRC)
	$(MKDIR) $(BUILD)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	$(RM) $(TARGET)