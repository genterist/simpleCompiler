# All of the sources participating in the build are defined here
-include targets.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

CC = gcc
OUT_PROG = comp
TEST_PROG = testScanner
DATA_FILE = parseResult

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: scanner

# Tool invocations
scanner: $(OBJS) $(USER_OBJS)
	@echo 'Building main program :' $(OUT_PROG)
	$(CC)  -o $(OUT_PROG) $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building main program :' $(OUT_PROG)
	@echo ' '
	
test: $(OBJS) $(USER_OBJS)
	@echo 'Building test program :' $(TEST_PROG)
	$(CC)  -o $(TEST_PROG) $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building test program :' $(TEST_PROG)
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) $(OUT_PROG) $(TEST_PROG) $(DATA_FILE)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
