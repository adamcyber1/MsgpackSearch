MsgpackSearch
====================================
[![Build Status](https://travis-ci.com/adamfill28/MsgpackSearch.svg?token=W9zuzwEoz6cKPPLYczY3&branch=master)](https://travis-ci.com/adamfill28/MsgpackSearch)

This library provides a JSON-like interface for serialized Msgpack data.

**ATTENTION, this software is in the alpha state!**

Any [feedback][mail] (*especially results of testing*) is highly appreciated!

Features
========

Interact with msgpack data as if it was JSON, eliminating the need to deserialize the data to make it searchable. This is
particularly useful when large blobs of Msgpack are only needed for a low number of key/index accesses. 

- Getter functions operate on the raw bytes - no copies required.

Examples
=======

```cpp

#include <msgpacksearch.h>
#include <vector>
#include <string>

using namespace msgpacksearch;

int main()
{
   /* 

   JSON: 
   {
        "A" : "hello",
        "B" : 0,
        "C" : [1 , 2, 3],
        "D" : {
            "NESTED": 4
        }
    }

    serialized msgpack: DF 00 00 00 04 A1 41 A5 68 65 6C 6C 6F A1 42 00 A1 43 DD 00 
                        00 00 03 01 02 03 A1 44 DF 00 00 00 01 A6 4E 45 53 54 45 44 
                        04
   */


   /*  Construction
   * 
   *   Msgpack class is a thin wrapper around the raw bytes and does not require ownership.
   */
   Msgpacksearch::Msgpack msgpack_data(buffer); 

   /* Getters
   * 
   * Should support operator[] syntax, as well as .get() functions. Access errors should be handled via throws (in the case of []) or null returns (in the case of .get())
   */
   msgpack_object value = msgpack_data["B"]; // returns std::variant with current alternative type 'uint64_t' and value 0

   uint64_t val_uint = msgpack_data.get_int("B"); // val_uint = 0;

   msgpack_object nested = msgpack_data["C"] // nested = msgpack_array([1, 2, 3])
   uint64_t nested_val = nested.get_int(0); // nested_val = 1
   
   return 0;
}
```

Installation 
==========================

Dependencies
------------

- [CMake] build system version 3.13+;
- C++17 compiler ([GCC] 7.3+)

Installation on Linux
---------------------

    $ git clone https://github.com/adamfill28/MsgpackSearch
    $ mkdir build && cd build
    $ cmake ..
    $ make && make install


Tests 
==========================
    $ ./build/test/msgpacksearch_unittest

License
=======

This library is distributed under the MIT license. For conditions of distribution and use,
see file `LICENSE.txt`.

Feedback
========

Any feedback are welcome. Feel free to make a pull request or submit and issue.

Copyright
=========

Adam Fillion 2019-2020

[CMake]: https://cmake.org/
[GCC]: https://gcc.gnu.org/