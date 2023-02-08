# Test log [windows]

Stdout+stderr (./omp4 0 in.pgm out0.pgm):

```
OK [program completed with code 0]
    [STDERR]:
    [STDOUT]: 78 131 188
Time (2 thread(s)): 55.4808 ms

```

Stdout+stderr (./omp4 -1 in.pgm out-1.pgm):

```
OK [program completed with code 0]
    [STDERR]:
    [STDOUT]: 78 131 188
Time (1 thread(s)): 56.3107 ms

```

Input image:

![Input image](test_data/in.png?sanitize=true&raw=true)

Output image 0:

![Output image 0](test_data/out.png?sanitize=true&raw=true)

Output image -1:

![Output image -1](test_data/out.png?sanitize=true&raw=true)
