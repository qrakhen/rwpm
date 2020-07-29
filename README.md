# RWPM
a simple C++ library for reading &amp; writing process memory.

## Usage
### Binding to a process
```C++
if (RWPM::init("process.exe")) {
    // getting the main module's base address
    DWORD dwMain = RWPM::getModule("process.exe");
    // ...or any other module's
    DWORD dwSalad = RWPM::getModule("salad.exe");
}
```

### Reading
```C++
float myFloat = RWPM::read<float>(dwMain + 0x72FE);
int alternate;
RWPM::read<int>(dwMain + 0x92AF, alternate);

struct vec3_t { 
  float x, y, z;
}
RWPM::read<vec3_t>(...); 
```
Pretty easy, right?

### Writing
```C++
long number = 6432168421;
RWPM::write<long>(dwMain + 0x1234, number);
```
