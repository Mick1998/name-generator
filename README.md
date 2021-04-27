# Name Generator for C++

[![Name Generator for C++](https://raw.githubusercontent.com/dasmig/name-generator/master/doc/name-generator.gif)](https://github.com/dasmig/name-generator/releases)

[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/dasmig/name-generator/master/LICENSE.MIT)
[![GitHub Releases](https://img.shields.io/github/release/dasmig/name-generator.svg)](https://github.com/dasmig/name-generator/releases)
[![GitHub Downloads](https://img.shields.io/github/downloads/dasmig/name-generator/total)](https://github.com/dasmig/name-generator/releases)
[![GitHub Issues](https://img.shields.io/github/issues/dasmig/name-generator.svg)](https://github.com/dasmig/name-generator/issues)

## Features

The library currently supports the following:

- **Name Generation**. The library generates a name randomly from a list of given names, organized through gender and country of origin.

- **Surname Generation**. The library generates a surname randomly from a list of surnames, organized through country of origin.

- **Full Name**. Easily append one or more names and surnames to generate a full name with an intuitive API.

- **Countries Supported**. Currently the following countries are supported by the library and contain a relevant database (unchecked countries are currently lacking or contain an incomplete database): 
  - [X] Australia
  - [X] Brazil
  - [X] Bulgaria
  - [X] Canada
  - [X] China
  - [X] Denmark
  - [X] Finland
  - [x] France
  - [X] Germany
  - [ ] Kazakhstan
  - [X] Norway
  - [ ] Poland
  - [ ] Russia
  - [X] Sweden
  - [X] Turkey
  - [ ] Ukraine
  - [X] United Kingdom
  - [x] United States of America


## Integration
 
[`namegen.hpp`](https://github.com/dasmig/name-generator/namegen.hpp) is the single required file [released here](https://github.com/dasmig/name-generator/releases). You need to add

```cpp
#include <dasmig/namegen.hpp>

// For convenience.
using ng = dasmig::ng;
```

to the files you want to generate names/surnames and set the necessary switches to enable C++17 (e.g., `-std=c++17` for GCC and Clang).

Additionally you must supply the name generator with the [`resources`](https://github.com/dasmig/name-generator/resources) folder also available in the [release](https://github.com/dasmig/name-generator/releases).

### Disclaimer

This README was heavily inspired by ['nlhomann/json'](https://github.com/nlohmann/json).