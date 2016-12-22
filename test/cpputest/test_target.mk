#compile tests
$(TEST_BIN_FOLDER)%.o: test/%.cpp
	@$(MKDIR_P) $(@D)
	$(CPP) -c $(CPPFLAGS) $(CPPUTEST_TEST_INCDIRS) $< -o $@

# link tests
$(TEST_BIN_FOLDER)cpputest/allTests : $(CPPUTEST_TEST_OBJS)
	$(CPP) $(CPPUTEST_TEST_OBJS) $(CPPUTEST_TEST_LDFLAGS) $(OPTIONS_ARCH) $(LIB) -o $@

# execute tests
cpputest_test: clean $(TEST_BIN_FOLDER)cpputest/allTests
	@echo ""
	@echo "Now running the tests"
	@echo "====================="
	@$(TEST_BIN_FOLDER)cpputest/allTests -c -v