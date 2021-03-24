all:build

build: main.obj hash_map.obj
	cl main.obj hash.obj -o so-cpp.exe

main.obj:main.c
	cl -c main.c -o main.obj

hash_map.obj:hash_map.c
	cl -c src\main\hash_map.c -o src\data_structures\hash_map.obj

clean:
	rm -rf main.obj hash_map.obj so-cpp.exe