WOPT=-Wall -Wextra -Wpedantic -pedantic-errors -pedantic -Werror
OPT=-std=c89 $(WOPT)

build: $(SRC)
	@ mkdir -p build/
	@ echo "[INFO] building..."	
	@ $(CC) -o ./build/dstr_app $(OPT) dstr_main.c

b: build

run: build
	@ echo "[INFO] running..."
	@ exec ./build/dstr_app

r: run

clean:
	@ echo "[INFO] cleaning..."
	rm -rf build/

c: clean

.PHONY: build b run r clean c
