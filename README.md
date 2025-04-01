# C++ STL Deque Implementation

This project implements a custom version of the C++ Standard Template Library
(STL) deque container. The implementation provides the same interface and
performance characteristics as `std::deque`.

## Overview

The custom deque is a double-ended queue that allows insertion and removal of
elements at both ends. Key features include:

- Template-based implementation supporting any data type
- Customizable allocator for memory management
- Efficient insertion and removal at both ends in amortized constant time
- Random access to elements
- Automatic memory management with dynamic resizing
- Implementation using an "array of arrays" approach
- Iterator support for traversing the container

## Class Structure

### `custom::deque<T, A>`

The main deque class template with two parameters:

- T: Type of elements
- A: Allocator type (default `std::allocator<T>`)

Key methods:

- `front()`, `back()`: Access the first and last elements
- `operator[]`: Random access to elements
- `push_front()`, `push_back()`: Insert elements at the beginning or end
- `pop_front()`, `pop_back()`: Remove elements from the beginning or end
- `size()`, `empty()`: Container info
- `begin()`, `end()`: Iterator access

## Implementation Details

The deque is implemented using an "array of arrays" approach, which provides:

- Efficient memory usage by allocating memory in blocks
- Fast random access to elements
- Constant time insertion and removal at both ends (amortized)
- No element copying during container growth, only copying of pointers

The implementation maintains:

- An array of pointers to blocks (`data`)
- A fixed number of cells per block (`numCells`)
- Tracking of the front element position (`iaFront`)
- Mapping between logical indices and physical storage locations

## Usage Example

```cpp
#include "deque.h"

// Create deque of integers
custom::deque<int> d;

// Add elements at both ends
d.push_back(3);
d.push_front(1);
d.push_back(4);
d.push_front(2);

// Access elements (output: 2)
int first = d.front();
std::cout << first << std::endl;

// Iterate through all elements
for (auto it = d.begin(); it != d.end(); ++it) {
    std::cout << *it << " ";
}
// Output: 2 1 3 4

// Remove elements from both ends
d.pop_front();
d.pop_back();
// Deque now contains: 1 3
```

## Testing

The implementation includes comprehensive unit tests in `testDeque.h` that
verify:

- Construction and destruction behavior
- Element access operations
- Push and pop functionality at both ends
- Iterator functionality
- Memory management
- Edge cases

Run tests by building in debug mode with the DEBUG flag defined.

## Files

- `deque.h`: Main deque implementation
- `testDeque.h`: Unit tests
- Other supporting files for testing framework

## Building

The project includes Visual Studio solution files for building on Windows. Open
the solution file and build using Visual Studio 2019 or later.

## Notes

- This is an educational implementation focused on demonstrating STL container
  concepts
- The implementation aims to match `std::deque`'s interface and performance
- Some interfaces have been excluded as they were implemented in previous data
  structures or didn't provide new learning opportunities

## License

This code is provided for educational purposes. See included license file for
terms of use. [TODO: add LICENSE]

## Authors

Nathan Bird  
[nathanbirdka@gmail.com](mailto:nathanbirdka@gmail.com)

Brock Hoskins  
[](.)
