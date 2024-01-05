HC=ghc
SOURCES=src/Main.hs
GEN_SOURCES=src/Lexer.x src/Parser.y
GENERATED=src/Lexer.hs src/Parser.hs
PACKAGE=B.zip

.PHONY: pack all run clean

all: main

run: main
	./main

clean:
	rm -rf src/*.o src/*.hi
	rm -rf $(GENERATED)
	rm -f main
	rm -Rf $(PACKAGE)

main: $(GENERATED) $(SOURCES)
	$(HC) -i./src -tmpdir . ./src/Main.hs -o main

$(GENERATED): $(GEN_SOURCES) $(SOURCES)
	alex src/Lexer.x -o src/Lexer.hs
	happy src/Parser.y -o src/Parser.hs

pack: $(GENERATED)
	zip $(PACKAGE) -r Makefile src

