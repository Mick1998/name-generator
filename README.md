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
  - [X] Argentina
  - [X] Brazil
  - [X] Bulgaria
  - [X] Canada
  - [X] China
  - [X] Denmark
  - [X] Finland
  - [x] France
  - [X] Germany
  - [X] Kazakhstan
  - [X] Mexico
  - [X] Norway
  - [X] Poland
  - [X] Portugal
  - [X] Russia
  - [x] Spain
  - [X] Sweden
  - [X] Turkey
  - [X] Ukraine
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

## Usage

It's important to note that due to the necessity of supporting multiple cultures characters and the way std::string works on windows, this library uses std::wstring to return the generated names.

When requesting a name for the first time the library will attempt to load the resource files containing each country databases (the default path is ./resources). It's important to manually load the resources folder if it's present in a different location. The library will recursively iterate through all entries in the loading directory, so only a single call to the root folder containing the name databases is necessary.

```cpp

using ng = dasmig::ng;

// Manually load the resources folder if necessary.
ng::instance().load("path//containing//names");

// Generates a name of any culture and any gender.
std::wstring name = ng::instance().get_name();

// Generates a female name of any culture.
std::wstring female_name = ng::instance().get_name(ng::gender::f);

// Generates a male name of bulgarian culture.
std::wstring bulgarian_name = ng::instance().get_name(ng::gender::m, ng::culture::bulgarian);

// Generates a male name of any gender and any culture and append two surnames of same culture.
std::wstring full_name = ng::instance().get_name().append_surname().append_surname();

// Output to stream a french female name with a surname.
std::wcout << ng::instance().get_name(ng::gender::f, ng::culture::french).append_surname();

```

## Planned Features

- **Deterministic Generation**. The user is capable of seeding the random generator.

- **Specialized Support for Surnames**. Appropriate handling of gendered surnames present in some cultures.

- **ISO 2-Letter Code**. API allowing request through countries 2-Letter code along the library enum.

### Disclaimer

Ukraine and Russia surnames are all in their male or neutral versions.

This README was heavily inspired by ['nlhomann/json'](https://github.com/nlohmann/json).