CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++11

numerics: main.o function.o expr.o
	g++ $(CPPFLAGS) -o numerics main.o function.o expr.o
main.o: main.cpp function.hpp expr.hpp
	g++ $(CPPFLAGS) -c main.cpp
function.o: function.cpp function.hpp expr.hpp
	g++ $(CPPFLAGS) -c function.cpp
expr.o: expr.cpp expr.hpp
	g++ $(CPPFLAGS) -c expr.cpp

.PHONY: clean
clean:
	rm -f numerics *.o *~
