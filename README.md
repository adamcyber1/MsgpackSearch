MsgpackSearch
====================================

This library provides a JSON-like interface for serialized Msgpack data.

**ATTENTION, this software is in the alpha state!**

Any [feedback][mail] (*especially results of testing*) is highly appreciated!

Features
========

Interact with msgpack data as if it was JSON, eliminating the need to deserialize the data to make it searchable.

- Getter functions operate on the raw bytes - no copies required.
- Setter functions ...........

Examples
=======

```cpp

#include <msgpacksearch.hpp>
#include <vector>

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

   std::vector<uint8_t> data = {}; //TODO populate with serialized msgpack

   Msgpacksearch msgpack_data(data.data());

   Mspacksearch::Value value = msgpack_data["A"]; // returns std::variant with current alternative type std::string and value "hello"
   Msgpacksearch::Value::Type type = value.type(); // returns enum type Mspacksearch::Value::Type::String

    return 0;
}
```

Installation 
==========================

Dependencies
------------

- [CMake] build system version 3.13+;
- C++17 compiler ([GCC] 7.3+)

Build time settings
-------------------

n/a

Installation on Linux
---------------------

    $ git clone https://github.com/adamfill28/MsgpackSearch
    $ mkdir build && cd build
    $ cmake ..
    $ make && make install


Tests 
==========================

There are no tests yet.

License
=======

This library is distributed under the XYZ license. For conditions of distribution and use,
see file `LICENSE.txt`.

Feedback
========

Any feedback are welcome. [Contact us][mail].

Copyright
=========

copyright info....

[mail]: mailto:fake@gmail.com

[CMake]: https://cmake.org/
[GCC]: https://gcc.gnu.org/
