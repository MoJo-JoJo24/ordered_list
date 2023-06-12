CC = g++
GDFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -g
GCFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -DNDEBUG -O3
INCLUDE = -I include -I ./
PROJECTSRC = src/testtools.cpp test/ordered_list_tests.cpp
remove_list = -f $(wildcard ./*.o) $(wildcard ./*.d) $(wildcard *.so) $(wildcard *.elf) $(wildcard vgcore.*) $(wildcard *.out)


efficiency_test.out : src/testtools.cpp test/efficiency_test.cpp
	$(CC) $(GCFLAGS) $(INCLUDE) $^ -pthread -o $@

functional_test.out : src/testtools.cpp test/functional_test.cpp
	$(CC) $(GCFLAGS) $(INCLUDE) $^ -pthread -o $@

.PHONY : clean
clean :
	rm $(remove_list)






