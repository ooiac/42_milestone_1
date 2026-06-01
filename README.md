# 42 School — Milestone 1

Welcome to **Milestone 1** of the 42 School curriculum! This milestone marks your transition from the foundational **libft** library to implementing essential I/O utilities that are fundamental to C programming. These projects teach you about string formatting, file I/O, variadic functions, and efficient buffering strategies.

## Overview

Milestone 1 consists of two complementary projects that form the backbone of many applications:

| Project | Type | Language | Key Concepts |
|---------|------|----------|--------------|
| **ft_printf** | I/O Utility | C | Variadic functions, string formatting, type conversion, parsing |
| **get_next_line** | File I/O | C | File descriptors, buffering, memory management, reading patterns |

---

## Project 1: ft_printf

### What It Is

**ft_printf** is a custom implementation of the standard C library function `printf()`. You must recreate the entire functionality of this ubiquitous function, handling format specifiers, type conversions, and diverse output formats.

The real printf is a marvel of programming—it can take any number of arguments of any type and format them according to a format string. This project teaches you how it does that.

### The Challenge

Unlike simpler functions, printf must:
1. **Parse the format string** to find format specifiers (%)
2. **Extract arguments** of unknown types from the variable argument list
3. **Convert types** appropriately (int to string, pointer to hex, etc.)
4. **Apply formatting** (width, precision, padding, alignment)
5. **Write output** to stdout

### Supported Format Specifiers

Your implementation handles these specifiers:

| Specifier | Output Type | Example |
|-----------|------------|---------|
| `c` | Character | `ft_printf("%c", 'A')` → `A` |
| `s` | String | `ft_printf("%s", "hello")` → `hello` |
| `d` or `i` | Signed integer (decimal) | `ft_printf("%d", -42)` → `-42` |
| `u` | Unsigned integer | `ft_printf("%u", 4294967295)` → `4294967295` |
| `x` | Hexadecimal (lowercase) | `ft_printf("%x", 255)` → `ff` |
| `X` | Hexadecimal (uppercase) | `ft_printf("%X", 255)` → `FF` |
| `p` | Pointer address (hex) | `ft_printf("%p", ptr)` → `0x7fff5fbff8c0` |
| `%` | Literal percent | `ft_printf("100%%")` → `100%` |

### How It Works

```
Format String Analysis
    ↓
Find '%' followed by specifier
    ↓
Extract corresponding argument from va_list
    ↓
Apply format rules (width, precision, flags)
    ↓
Convert to string representation
    ↓
Write to stdout
    ↓
Track characters written
    ↓
Return total character count
```

### Key Concepts

#### Variadic Functions
```c
int ft_printf(const char *format, ...)
```

The `...` means "accept any number of arguments of any type". Inside the function:
- `va_list args`: Declares a list to hold the arguments
- `va_start(args, format)`: Initialize the list
- `va_arg(args, TYPE)`: Extract the next argument as TYPE
- `va_end(args)`: Clean up

#### Type Conversions
- **Integer to string**: Convert decimal, hex, octal, etc.
- **Pointer to hex**: Cast to `unsigned long`, convert to hex
- **Integer to character**: Take only the first byte
- **String handling**: Check for NULL, copy safely

#### Format Flags (Bonus)
Many implementations support additional flags:
- `-`: Left-align output
- `0`: Pad with zeros instead of spaces
- `+`: Always show sign for numbers
- ` ` (space): Prefix positive numbers with space
- `#`: Alternate form (0x for hex, 0 for octal)
- Width specifiers: `%10d` (10 character width)
- Precision: `%.5s` (max 5 characters for strings)

### Example Usage

```bash
cd ft_printf
make
```

Then in your code:
```c
ft_printf("Hello %s, you are %d years old\n", "Alice", 25);
// Output: Hello Alice, you are 25 years old

ft_printf("Hex: %x, Uppercase: %X, Pointer: %p\n", 255, 255, ptr);
// Output: Hex: ff, Uppercase: FF, Pointer: 0x7fff5fbff8c0

int chars = ft_printf("Count: %d\n", 42);
// Returns: 9 (number of characters printed)
```

### Learning Outcomes

- **Variadic functions**: Understanding how functions accept variable arguments
- **String manipulation**: Parsing, formatting, padding
- **Type casting**: Converting between different data types
- **Bit manipulation**: Especially for hexadecimal conversion
- **Standard library functions**: How printf really works
- **Code organization**: Breaking complex logic into helper functions

### Compilation

```bash
cd ft_printf
make              # Compile the project
make clean        # Remove object files
make fclean       # Remove all generated files
```

---

## Project 2: get_next_line

### What It Is

**get_next_line** implements a function that reads one line at a time from a file descriptor. It handles the complexity of reading arbitrary-length lines from a file, managing internal buffering, and handling multiple file descriptors simultaneously.

This is one of the most practical projects in 42—many subsequent projects depend on it.

### The Challenge

Reading a single line from a file sounds simple until you realize:
1. Files can have lines of any length
2. You must handle multiple file descriptors independently
3. You need to buffer efficiently (not read byte-by-byte, but not allocate huge chunks)
4. You must handle EOF, errors, and edge cases
5. Memory must be managed correctly (no leaks, no overwrites)

### How It Works: The Buffering Strategy

The key insight is a **static buffer** that persists between function calls:

```
Call 1:
  Read BUFFER_SIZE bytes into buffer
  Find newline
  Extract line
  Save remainder for next call
  Return line

Call 2:
  Use remainder from previous call
  Find newline (already in buffer!)
  Extract line
  Return line

Call 3:
  Remainder used up
  Read more bytes
  Continue...
```

### Function Signature

```c
char *get_next_line(int fd);
```

- **Input**: File descriptor (0 for stdin, or opened file)
- **Output**: The next line (including newline if present), or NULL if EOF/error
- **Memory**: Allocated by function, must be freed by caller

### Example Usage

Reading from a file:
```c
int fd = open("file.txt", O_RDONLY);
char *line;

while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);  // Line already includes \n
    free(line);
}
close(fd);
```

Reading from stdin:
```c
char *line;
while ((line = get_next_line(0)) != NULL)  // 0 is stdin
{
    printf("You wrote: %s", line);
    free(line);
}
```

### Key Implementation Details

#### Static Buffer Management
```c
static char *remainder = NULL;
```

This persists across function calls for the same fd. However, in the full implementation, you might maintain separate buffers per fd (if handling multiple files).

#### Line Extraction Logic
1. Check if remainder has a newline
2. If not, read more data from fd
3. When newline found, split the buffer
4. Return the line portion
5. Save the rest for next call

#### Memory Considerations
- Allocate exactly the size needed (no waste)
- Handle reallocation as lines grow
- Free old buffers appropriately
- Check all malloc returns

### BUFFER_SIZE Constant

```c
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif
```

BUFFER_SIZE is recommended to be 42 (as a nod to the school), but it's configurable at compile time:
```bash
gcc -D BUFFER_SIZE=1024 -c get_next_line.c
```

Different buffer sizes have tradeoffs:
- Small (1, 10): More reads, slower reading
- Medium (42, 100): Good balance
- Large (4096, 65536): Fewer reads, more memory per fd

### Handling Multiple File Descriptors (Bonus)

The bonus version must correctly handle reading from multiple file descriptors simultaneously:

```c
char *line1 = get_next_line(3);  // Read from fd 3
char *line2 = get_next_line(5);  // Read from fd 5
char *line3 = get_next_line(3);  // Continue from fd 3
```

This requires careful buffer management—typically a linked list of buffers, one per fd.

### Edge Cases to Handle

- EOF reached (return remaining text or NULL)
- File has no newlines (return entire content)
- Empty files (return NULL)
- Read errors (return NULL)
- Very long lines (multiple buffer fills)
- Empty lines (just `\n`)
- Multiple files open simultaneously
- BUFFER_SIZE of 1 (forces many reads)

### Learning Outcomes

- **File descriptors**: Low-level file operations with `read()`
- **Buffering strategies**: Efficient I/O patterns
- **Static variables**: Persistent state across function calls
- **Memory allocation**: Careful malloc and realloc usage
- **String manipulation**: Splitting and joining strings
- **Edge case handling**: Robust code that handles errors gracefully

### Compilation

```bash
cd get_next_line
gcc -D BUFFER_SIZE=42 -c get_next_line.c
gcc -D BUFFER_SIZE=42 -c get_next_line_utils.c
# Then link with your project
```

---

## How These Projects Work Together

### Complementary Functions

| Operation | ft_printf | get_next_line |
|-----------|-----------|---------------|
| Output | Writes formatted data TO stdout | Reads data FROM file |
| Memory | Minimal allocation | Persistent buffer allocation |
| Complexity | Parsing format string | Managing file descriptor buffers |
| Reusability | Used in almost every project | Used in many file-reading projects |

### Typical Usage Pattern

```c
// Read lines from file and print them formatted
int fd = open("data.txt", O_RDONLY);
char *line;

while ((line = get_next_line(fd)) != NULL)
{
    ft_printf("Line: %s", line);  // Already has \n
    free(line);
}
close(fd);
```

---

## Common Pitfalls & Solutions

### ft_printf Pitfalls

1. **Integer overflow**: Very large numbers may overflow—handle carefully
2. **NULL pointer for %s**: Standard printf prints "(null)" for NULL strings
3. **Off-by-one errors**: Exact character counting must be accurate
4. **Format specifier conflicts**: %x vs %X, %d vs %i edge cases
5. **Memory leaks**: If allocating for conversions, free appropriately

### get_next_line Pitfalls

1. **Forgetting to free memory**: Caller must `free()` each returned line
2. **Buffer overflows**: Always check buffer sizes
3. **Not handling EOF correctly**: Must distinguish EOF from error
4. **Static buffer issues**: If using global state, can cause issues with multiple files
5. **BUFFER_SIZE = 1**: Edge case that requires reading one byte at a time
6. **Memory leaks in failure cases**: Must clean up on read errors

---

## Curriculum Context

| Milestone | Projects | Focus |
|-----------|----------|-------|
| 0 | libft | Build foundational C library |
| **1** | **ft_printf, get_next_line** | **I/O utilities and string formatting** |
| 2 | push_swap, minitalk, fract-ol | Algorithms, IPC, graphics |
| 3 | minishell, philosophers | Systems programming & concurrency |
| 4 | cub3d | Advanced graphics (3D ray-casting) |

Milestone 1 is where you stop implementing helper functions and start implementing real tools. Many projects from Milestone 2 onward use ft_printf for debugging and get_next_line for input handling.

---

## Resources & References

### ft_printf
- `printf(3)`: `man printf`
- `va_arg(3)`: `man va_arg` - Variadic function macros
- Format specifier details in standard C library documentation
- ASCII table for character conversions

### get_next_line
- `read(2)`: `man read` - Reading from file descriptors
- `open(2)`, `close(2)`: File operations
- Static variables in C: Persistent local variables
- Bonus: Linked lists for multiple file descriptor handling

---

## Tips for Success

1. **ft_printf**:
   - Start with simple specifiers (c, s) before complex ones (p, x)
   - Test each specifier independently
   - Use printf itself for debugging (before you complete your version!)
   - Handle edge cases early (NULL strings, negative numbers, zero)

2. **get_next_line**:
   - Draw diagrams of the buffer state as you read lines
   - Test with BUFFER_SIZE=1 to verify byte-by-byte reading works
   - Test with multiple file descriptors open
   - Create test files with various line lengths and endings

3. **Both Projects**:
   - Write helper functions for complex operations
   - Test extensively with edge cases before submission
   - Avoid global variables (use static locals when possible)
   - Memory must be clean (no leaks, no uninitialized reads)

---

## Checklist for Success

### ft_printf
- [ ] All format specifiers work: c, s, d, i, u, x, X, p, %
- [ ] Return value matches actual character count
- [ ] NULL string handling works (%s with NULL)
- [ ] Pointer addresses print correctly in hex
- [ ] Signed and unsigned integers work
- [ ] Hexadecimal (lowercase and uppercase) correct
- [ ] No memory leaks
- [ ] Handles edge cases (0, negative numbers, large numbers)

### get_next_line
- [ ] Reads one complete line per call
- [ ] Returns line including newline (if present)
- [ ] Returns NULL at EOF
- [ ] Returns NULL on error
- [ ] Handles files with very long lines
- [ ] Handles files with no trailing newline
- [ ] BUFFER_SIZE=1 works correctly
- [ ] Memory is freed by caller (no leaks in get_next_line)
- [ ] Multiple file descriptors work independently
- [ ] Works with stdin (fd=0)

---

## Bonus Features

### ft_printf Bonus
Many implementations include additional format specifiers and flags:
- Width and precision: `%10d`, `%.5s`
- Padding: `%-10d` (left align), `%010d` (zero pad)
- Flags: `%+d` (always show sign), `% d` (space for positive)
- Alternate form: `%#x` (0x prefix)

### get_next_line Bonus
- Handle multiple file descriptors simultaneously
- Support various BUFFER_SIZE values
- Optimized performance with large files
- Robust error handling with proper cleanup

---

**Congratulations on reaching Milestone 1! These projects are foundations for all your future work at 42.**
