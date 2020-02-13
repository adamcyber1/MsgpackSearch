MsgpackSearch
====================================
[![Build Status](https://travis-ci.com/adamfillion/MsgpackSearch.svg?token=W9zuzwEoz6cKPPLYczY3&branch=master)](https://travis-ci.com/adamfillion/MsgpackSearch)

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
#include <string>

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

   /*  Construction
   * 
   *   Msgpack class is a thin wrapper around the raw bytes and does not require ownership.
   */
   Msgpacksearch::Msgpack msgpack_data(data); 

   /* Getters
   * 
   * Should support operator[] syntax, as well as .get() functions. Access errors should be handled via throws (in the case of []) or null returns (in the case of .get())
   * 
   */
   Msgpacksearch::Value value = msgpack_data["A"]; // returns std::variant with current alternative type 'std::string' and value "hello"
   Msgpacksearch::Value::Type type = value.type(); // returns enum type Mspacksearch::Value::Type::String via std::variant<Types...> index

   std::string A = msgpack_data["A"].get<std::string>(); //gets a Value type (which is a wrapper around std::variant), then using accesses the std::string alternative type.
   std::string A2 = msgpack_data.get<std::string>("A") // you should be rewarded for knowing the data type beforehand.

   Msgpacksearch::Value nested = msgpack_data["D"] // returns std::variant with current alternative type 'Msgpacksearch::Value' and value msgpack([1, 2, 3]).. (should this return an Array type)
   int nested_val = nested[0].get<int>() // returns int 1 from "C": 


   /* Setters
   *
   * TBD.
   */

    /* Misc */   
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
