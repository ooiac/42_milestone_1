# Milestone 1 — ft_printf and get_next_line

Two I/O projects from the 42 Common Core, written in C. `ft_printf` formats and writes data; `get_next_line` reads a file descriptor one line at a time. Both depend on careful handling of buffers, return values, and edge cases.

## ft_printf

Source: [`ft_printf/`](ft_printf/)

This is an implementation of the required subset of `printf`, not every flag and conversion supported by the system's libc. It scans a format string, writes normal characters directly, and dispatches `%` conversions to the appropriate formatting function.

| Conversion | Meaning | Example |
| --- | --- | --- |
| `%c` | Character | `A` |
| `%s` | String | `hello` |
| `%d`, `%i` | Signed decimal integer | `-42` |
| `%u` | Unsigned decimal integer | `42` |
| `%x`, `%X` | Lower/uppercase hexadecimal | `ff`, `FF` |
| `%p` | Pointer address in hexadecimal | `0x...` |
| `%%` | Literal percent sign | `%` |

### How the arguments are read

The function takes a format string followed by an unknown number of arguments:

```c
int ft_printf(const char *format, ...);
```

C handles that through `<stdarg.h>`:

```c
va_list args;

va_start(args, format);
/* inspect format, then retrieve the matching type with va_arg */
va_end(args);
```

The conversion character tells the implementation which type to request with `va_arg`. Passing the wrong type is not something `ft_printf` can automatically detect. Integers must be converted to decimal or hexadecimal digits, and `%p` needs an address-sized unsigned representation.

The return value is the **number of characters written**, not the number of format specifiers or arguments. For example:

```c
int n = ft_printf("Value: %d\n", 42);
/* Prints "Value: 42" followed by a newline; n is 10. */
```

Important cases include a `NULL` string, negative numbers, zero, the full unsigned range, literal `%%`, and correctly counting every printed character. Width, precision, and flag handling are not part of the mandatory conversion list above.

### Build

```sh
cd ft_printf
make
# Produces a static library that can be linked into a C program.
make clean
make fclean
```

## get_next_line

Source: [`get_next_line/`](get_next_line/)

```c
char *get_next_line(int fd);
```

The function returns the next line from `fd`, **including its trailing newline when there is one**. The returned line is allocated; the caller has to free it. It returns `NULL` at the end of the file or when reading fails.

### Why it needs a persistent buffer

A single `read(fd, buffer, BUFFER_SIZE)` is not guaranteed to end on a newline. It may contain half a line or several lines at once. `get_next_line` therefore keeps the unused part between calls.

```text
File:             first line\nsecond line\nlast line
read() chunk:     first line\nsec
returned line:   first line\n
saved remainder:  sec
next read():               ond line\nlast...
```

The usual sequence is: check the saved remainder, read until a newline or EOF, extract one line, and keep everything after it for the next call. Long lines may take many reads and concatenations. The bonus version needs independent saved state for each file descriptor so alternating between two files doesn't mix their contents.

### BUFFER_SIZE

`BUFFER_SIZE` controls how many bytes are requested per read; it does not limit the final line length. It can be set when compiling:

```sh
cd get_next_line
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    your_main.c get_next_line.c get_next_line_utils.c -o test_gnl
```

A buffer size of 1 exercises the most reads; a larger buffer reduces system calls but can leave more data waiting in the saved remainder. Test both, including a line longer than `BUFFER_SIZE`.

### Example

```c
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

int main(void)
{
    int fd = open("file.txt", O_RDONLY);
    char *line;

    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        /* Process the line here. */
        free(line);
    }
    close(fd);
    return (0);
}
```

Also check empty files, files without a final newline, blank lines (`"\n"`), invalid descriptors, and read errors. A function that passes the normal cases but leaks the remainder on an error still needs fixing.

## Using both

A program can read with `get_next_line` and print the result with `ft_printf`. The important distinction is ownership: `ft_printf` writes its result; `get_next_line` hands the caller memory that must eventually be freed.

Useful references: `man 3 printf`, `man 2 read`, `man 3 stdarg`, and `man 2 open`.
