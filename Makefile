TARGET	= ggsampleB
SOURCES	= $(wildcard *.cpp) $(wildcard lib/*.cpp)
HEADERS	= $(wildcard *.h) $(wildcard lib/*.h)
OBJECTS	= $(patsubst %.cpp,%.o,$(SOURCES))
CXXFLAGS	= --std=c++17 -g -Wall -DDEBUG -DX11 -Iinclude
GTKLIBS	= `pkg-config --libs gtk+-3.0`
LDLIBS	= -Llib -lglfw3_linux -lnfd_linux -lGL -lXrandr -lXinerama -lXcursor -lXi -lX11 -lpthread -lrt -lm -ldl $(GTKLIBS)

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(TARGET).dep: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -MM $(SOURCES) > $@

clean:
	-$(RM) $(TARGET) *.o lib/*.o *~ .*~ *.bak *.dep imgui.ini a.out core

-include $(TARGET).dep
