TARGET = sample
LEXER_SRC = lexer.cpp
PARSER_SRC = parser.cpp
SRCFILE =  $(PARSER_SRC) $(LEXER_SRC) main.cpp
OBJFILES =${SRCFILE:.cpp = .o}

$(TARGET):$(OBJFILES)
	g++ -std=c++11 -o $@ $^

%.o : %.cpp tokens.h
	g++ -Wall -std=c++11 -c -o $@ $<

$(PARSER_SRC): grammar.y
	bison -v --defines=tokens.h -o $@ $^

$(LEXER_SRC): lexer.l
	flex -o $@ $<

run: $(TARGET)
	./$(TARGET) input.txt
