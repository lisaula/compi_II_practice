TARGET = sample
LEXER_SRC = lexer.cpp
GRAMMAR_SRC = grammar.cpp
SRCFILES = $(GRAMMAR_SRC) $(LEXER_SRC) main.cpp
OBJECT_FILES = ${SRCFILES:.cpp = .o}

$(TARGET): $(OBJECT_FILES)
	g++ -o $@ $^

$(LEXER_SRC): lexer.l
	flex -o $@ $<

$(GRAMMAR_SRC): grammar.y
	bison -v --defines=tokens.h -o $@ $<

%.o: %.cpp tokens.h
	g++ -c -o $<

run:
	./$(TARGET) < input.txt
