all:build

build: main hash_map
	cl .\main.obj .\hash_map.obj -o .\so-cpp.exe

main:
	cl .\main.obj -c .\src\main\main.c

hash_map:
	cl .\hash_map.obj -c .\src\data_structures\hash_map.c

clean:
	rm main.obj hash_map.obj so-cpp.exe
