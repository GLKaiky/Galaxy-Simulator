CXX      := g++
TARGET   := galaxy_sim
BUILD    := build

# Todos os .cpp do projeto (ignora build/ e lib/)
SRCS := $(shell find . -name '*.cpp' -not -path './$(BUILD)/*' -not -path './lib/*')
OBJS := $(SRCS:./%.cpp=$(BUILD)/%.o)
DEPS := $(OBJS:.o=.d)

# -MMD -MP gera arquivos .d para rastrear mudanças em headers
CXXFLAGS := -std=c++17 -Wall -Wextra -fopenmp -I./include -MMD -MP
LDFLAGS  := -fopenmp -L./lib -flto
LDLIBS   := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# make          -> Release (padrão)
# make debug    -> Debug (com símbolos, sem otimização)
CXXFLAGS += -O3 -march=native  -ffast-math -flto=auto

.PHONY: all debug run clean

all: $(TARGET)

debug: CXXFLAGS := $(filter-out -O3 -march=native,$(CXXFLAGS)) -O0 -g
debug: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET)

-include $(DEPS)