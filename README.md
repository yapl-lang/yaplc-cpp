# YAPLC
Yet Another Programming Language compiler. Compiles yapl to C and to other language in future.

# Building
Install cmake:
```
	apt-get install cmake
```

Clone repository:
```
	git clone git@github.com:Tarik02/yaplc.git
	cd yaplc
```

### Building for PC
If you want to build, just type:
```
	./yad build
```
It will search cmake or make to build yaplc.

Also you can build yad using given building system:
```
	./yad build cmake # Will build yaplc using cmake
	./yad build make # Will build yaplc using make
```

### Building for Android
Install NDK first and then type:
```
	./yad build ndk
```