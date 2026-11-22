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