# How to Compile

```bash
mkdir build && cd build
cmake ..
make -j4
./Mutatris -p ../assets
```

# How to compile with emscripten

First download the rquired libs from the Releases section
or compile libpng and zlib and put it in emscripten-libs then:


```bash
make -f Makefile.em -j4
```

test it
```bash
python3 -m http.server 3000
```

open browser to localhost:3000

