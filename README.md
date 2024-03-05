
# An Optimized String Implementation( in this branch we try to make it)

The mjz_ard::basic_string class focuses on memory efficiency through optimizations like:

- Small String Optimization (SSO) 
- Copy on Write (COW)
- Shared substrings
- Deferred null placement
- Precomputed hashes

It fits within 32 bytes on 64-bit systems using a clever memory layout. A single "control byte" encodes string type, encoding, length/capacity flags and more.

This allows:

- SSO up to 31 characters 
- Multiple encodings like UTF-8  
- Large strings up to 2^60 characters
- String literal view compatibility
- Atomic operations for concurrency
- Custom allocators

Performance is improved via:

- Avoiding reallocation and copying through SSO, COW and shared substrings
- Precomputed hashes for large strings  
- Atomic heap access for threads

A basic_rope class handles very large strings as a vector of basic_strings, tracking differences to reduce copying during modifications.

While optimized for memory efficiency and performance, the complexity is higher than a simpler string class. 

The key techniques are:

- SSO stores short strings in stack memory
- COW copies strings only on modification, sharing underlying data
- Shared substrings allow multiple strings to reference the same data
- Deferred null placement reduces allocation for some strings    
- A "null trick" control byte stores capacity and acts as null terminator for SSO strings
- String literal view compatibility avoids copying static strings
- An "invalid" state marks strings to avoid throwing exceptions

Memory efficiency was the top priority over unnecessary features. This demonstrates how far optimizations can be pushed within a small footprint to provide an efficient string for many use cases.

Overall, the mjz_ard::basic_string class provides an optimized string implementation focused on memory efficiency, performance and large string support through techniques like SSO, COW, shared substrings and deferred null placement.
