# How to Compile

```bash
mkdir build && cd build
cmake ..
make -j4
./Mutatris -p ../assets
```

# How to compile with emscripten

```bash
make -f Makefile.em -j4
```

test it
```bash
python3 -m http.server 3000
```

open browser to localhost:3000

