EXEC=main.exe
GPP=g++
FLAGS=-std=c++20 -Wall -Wextra -pedantic

$(EXEC): reader.cpp testprint.cpp createclass.cpp generator.cpp Api.cpp
	$(GPP) $(FLAGS) $? -o $(EXEC)

.PHONY:
run: $(EXEC)
	./$(EXEC) testing.csv

.PHONY:
clean:
	@rm -f $(EXEC)
