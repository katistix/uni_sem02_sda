# 1. Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = App
SRCS = App.cpp Matrice.cpp TestExtins.cpp TestScurt.cpp

.PHONY: all clean run

# 2. Default rule
all: $(TARGET)

# 3. Compile the app
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# 4. Run, then automatically clean
run: $(TARGET)
	-@./$(TARGET)
	@echo "\n\nCleaning up..."
	@$(MAKE) clean

# 5. Remove the executable
clean:
	rm -f $(TARGET)