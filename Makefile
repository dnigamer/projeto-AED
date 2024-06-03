BUILD_DIR = build
TARGET = Projeto

all: build run

cmake:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

build: cmake
	@$(MAKE) -C $(BUILD_DIR)

run:
	@$(BUILD_DIR)/$(TARGET)

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all cmake build run clean
