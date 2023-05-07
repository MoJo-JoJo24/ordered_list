CC = g++
GDFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -g
GCFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -DNDEBUG -O3
INCLUDE = -I include -I ./
PROJECTSRC = src/testtools.cpp test/ordered_list_tests.cpp
remove_list = -f $(wildcard ./*.o) $(wildcard ./*.d) $(wildcard *.so) $(wildcard *.elf) $(wildcard vgcore.*) $(wildcard *.out)


project_debug : $(PROJECTSRC) 
	$(CC) $(GDFLAGS) $(INCLUDE) $^ -pthread -o ordered_list_tests.out 

project_release : $(PROJECTSRC) 
	$(CC) $(GCFLAGS) $(INCLUDE) $^ -pthread -o ordered_list_tests.out

.PHONY : clean
clean :
	rm $(remove_list)






