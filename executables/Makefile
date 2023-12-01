exec = temp
sources = $(wildcard ../scr/lexer/*.c) $(wildcard ../scr/*.c) $(wildcard ../scr/parser/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -ldl -fPIC -rdynamic 

$(exec):	$(objects)
	gcc $(objects) $(flags) -o $(exec)

lib.a: $(objects)
		ar rcs $@ $^
%.o: %.c ../scr/include/%.h
		gcc -c $(flags) $< -o $@

# install:
# 	make
# 	make lib.a
# 	mkdir -p /usr/local/include/
# 	cp -r ./scr/include/* /usr/local/include//.
# 	cp ./lib.a /usr/local/lib/.
# 	cp ./.out /usr/local/bin/

#$(wildcard ../scr/parser/*.o)
clean:
	-rm -f $(wildcard ../scr/lexer/*.o) $(wildcard *.a) $(wildcard ../scr/*.o) $(wildcard ../scr/parser/*.o)

lint:
	clang-tidy lexer/*.c include/*.h
