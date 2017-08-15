TARGET=CompiI
EXPR_LEXER=expression_lexer.cpp
EXPR_PARSER=expression_parser.cpp

SRC_FILES=$(EXPR_LEXER) $(EXPR_PARSER) main.cpp ast.cpp

OBJ_FILES=$(SRC_FILES: .cpp=.o)

.PHONY: clean

$(TARGET): $(OBJ_FILES)
	g++ -std=c++11 -o $@ $^

$(EXPR_LEXER): lexer.l
	flex -o $@ $^

$(EXPR_PARSER): grammar.y
	lemon -o$@ -dtokens.h -Tlempar.c $^

%.o: %.cpp tokens.h
	g++ -std=c++11 -c %<

run: $(TARGET)
	./$(TARGET) < input.txt

clean:
	rm -rf *.o
	rm -rf *.output
	rm -rf $(EXPR_LEXER) $(EXPR_PARSER)
	rm -rf tokens.h