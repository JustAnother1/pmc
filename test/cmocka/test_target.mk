#compile tests
$(TEST_BIN_FOLDER)%o: test/%c
	@$(MKDIR_P) $(@D)
	$(CC) -c $(CFLAGS) $(DDEFS) $(OPTIONS_ARCH) $(CMOCKA_TEST_INCDIRS) $< -o $@

# link tests
$(TEST_BIN_FOLDER)cmocka/src/debug/test_debug : $(CMOCKA_SRC_DEBUG_DEBUG_OBJS)
	$(CC) $(CMOCKA_SRC_DEBUG_DEBUG_OBJS) $(CMOCKA_TEST_LDFLAGS) $(OPTIONS_ARCH) $(LIB) -o $@

$(TEST_BIN_FOLDER)cmocka/src/stepper/test_step : $(CMOCKA_SRC_STEPPER_STEP_OBJS)
	$(CC) $(CMOCKA_SRC_STEPPER_STEP_OBJS) $(CMOCKA_TEST_LDFLAGS) $(OPTIONS_ARCH) $(LIB) -o $@

# execute tests
cmocka_test: clean $(TEST_BIN_FOLDER)cmocka/src/debug/test_debug $(TEST_BIN_FOLDER)cmocka/src/stepper/test_step
	CMOCKA_XML_FILE=cm_debug.xml $(TEST_BIN_FOLDER)cmocka/src/debug/test_debug
	CMOCKA_XML_FILE=cm_step.xml $(TEST_BIN_FOLDER)cmocka/src/stepper/test_step