build:
	gcc -g -Wall -pedantic lang.c -o lang

compile-ir:
	clang --target=wasm32 -emit-llvm -c -S $(file_name)

object-file:
	llc -march=wasm32 -filetype=obj $(file_name)
