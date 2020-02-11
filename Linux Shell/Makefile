RULES = -Wall -Werror -pedantic -std=gnu++98

ffosh: ffosh.cpp argProc.hpp argProc.cpp myFunc.hpp myFunc.cpp
	g++ -o ffosh $(RULES) ffosh.cpp argProc.hpp argProc.cpp myFunc.hpp myFunc.cpp
.PHONY: clean
clean:
	rm -f ffosh *~ *.o *.gch
