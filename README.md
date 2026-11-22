## How to run on windows

### run your docker env
```cmd
docker run --rm -it -v "${PWD}:/work" -w /work gcc:latest bash
```

### compile
```cmd
gcc -Wall -Wextra -std=c11 main.c allocator/allocator.c -o allocator_test
```

### execute
```cmd
./allocator_test
```

### expected output
A: Hello from my allocator!\
B: Memory management in C\
D: Allocation successful!

### expected output with debug prints:

```cmd
Block 0 | Size: 112 | Free: No
Block 1 | Size: 208 | Free: No
Block 2 | Size: 304 | Free: No
Block 3 | Size: 1047696 | Free: Yes
A: Hello from my allocator!
B: Memory management in C
Block 0 | Size: 384 | Free: Yes
Block 1 | Size: 304 | Free: No
Block 2 | Size: 1047696 | Free: Yes
Block 0 | Size: 256 | Free: No
Block 1 | Size: 64 | Free: Yes
Block 2 | Size: 304 | Free: No
Block 3 | Size: 1047696 | Free: Yes
D: Allocation successful!
root@71703212c94f:/work# 
```
