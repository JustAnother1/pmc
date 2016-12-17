#compile tests
$(TEST_BIN_FOLDER)%o: %c
	@$(MKDIR_P) $(@D)
	$(CC) -c $(CFLAGS) $(DDEFS) $(OPTIONS_ARCH) $(TEST_INCDIR) $< -o $@

# link tests
$(TEST_BIN_FOLDER)test/cmocka/src/debug/test_debug : $(TEST_OBJS)
	$(CC) $(TEST_OBJS) $(TEST_LDFLAGS) $(OPTIONS_ARCH) $(LIB) -o $@

# execute tests
test: clean $(TEST_BIN_FOLDER)test/cmocka/src/debug/test_debug
	$(TEST_BIN_FOLDER)test/cmocka/src/debug/test_debug