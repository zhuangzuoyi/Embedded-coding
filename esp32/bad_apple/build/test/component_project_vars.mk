# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/ssd1306/test/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/test -Wl,--whole-archive -ltest -Wl,--no-whole-archive
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += test
component-test-build: 
