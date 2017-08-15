
TARGET= lemon_sample
EXPR_PARSER_SRC= expr_parser.cpp
EXPR_LEXER_SRC= expr_lexer.cpp
SRCFILES = $(EXPR_PARSER_SRC) $(EXPR_LEXER_SRC) ast.cpp main.cpp
OBJ_FILES=${SRCFILES:.cpp = .o}
.PHONY: clean

$(TARGET): $(OBJ_FILES)
	g++ -o $@ $(OBJ_FILES)

$(EXPR_LEXER_SRC): expr.l
	flex -o $@ $^

$(EXPR_PARSER_SRC): gram.y
	./lemon -o$@ -dtoken.h -Tlempar.c $<

%.o: %.cpp token.h
	g++ -c -o $@ $<

run: $(TARGET)
	./$(TARGET) input.txt

clean:
	rm -f $(EXPR_PARSER_SRC) $(EXPR_LEXER_SRC)
	rm -f *.o
	rm -f token.h
	rm -f $(TARGET)
