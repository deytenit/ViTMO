# OneMoreFunnyLanguage format

OMFL - it is a format for storing configuration files. It is similar in nature to ini, but more flexible. The main goal is the ability to get the value from the config by the universal key and the logical division into sections and subsections.

The following library contains **parser**, **stringifyer** and **jsonifier**

## Format

An OMFL file is an ASCII encoded text file in the OMFL format. Next, the format specification will be described. The basic construct for OMFL is the _key-value_ pair.
The format is case sensitive. Spaces and empty lines are ignored.

### Key-Value

The key is on the left, then the equal sign, then the value.

```ini
key = "value"
```

Both the key and value are required. Line break is prohibited.
Value cannot be redefined for the same key (within the same section).

### Key

The key may be:

- Uppercase and lowercase Latin letters
- Digit
- Characters '-' and '\_'

The key cannot be zero length.

```ini
number = 0
name = "M3100"
```

### Value

The value can be one of the following types

- int
- double
- string
- bool
- array of values stated

## Types

### Int

Consists of numbers _(one or more)_. It is possible to add a **'+'** or **'-'** character as the first character.

```ini
key1 = 1
key2=-2022
key3 = +128
```

Possible value will fit in _int32_t_.

### Double

Consists of digits _(one or more)_ and one **'.'**. There must be at least one digit before and after the dot
It is possible to add a **'+'** or **'-'** character as the first character.

```ini
key1 = 1.23
key2 = -2.77
key3 = -0.0001
```

### String

The string is surrounded by double quotes. Contains any characters.

```ini
key1 = "value"
key2 = "Hello world!"
```

### Bool

Can be either _true_ or _false_.

```ini
key1 = true
key2 = false
```

### Array

The array is surrounded by the characters **'['** and **']'**. Elements are separated by **','**.
An array can contain any valid Values, **not necessarily** of the same type.

```ini
key1 = [1, 2, 3, 4, 5]
key2 = ["Hello", "world"]
key3 = [[1, 2, 3, 4, 5], ["Hello", "world"]]
key4 = [1, 3.3, "ITMO", [true, false]]
```

## Sections

In addition to the key-value block, the format supports sections. Sections allow you to combine key-value sets into logical blocks.
A section is defined as a Key surrounded by **'['** and **']'**

```ini
[name]
```

After a section is declared, all subsequent key-value pairs belong to that section until the next one is declared.

```ini
[section-1]
key1 = 1
key2 = "2"

[section-2]
key1 = 2
```

Although the section is subject to the fusion of Keys, it may also contain the character **'.'**.
This one defines **nested** sections.

```ini
[section-1.part-1.x]

[section-1.part-1.y]

[section-1.part-2.x]

[section-2.z]
```

Thus, a section can contain both key-value pairs and other sections. Key and subsection name cannot match

```ini
[A]
key1 = 1

[A.B]
key2 = 3

[A.B.C]
key3 = 3
```

## Comments

The config can contain a single line comment. Comments start with a **'#'** unless is inside a string value.

```ini
    key1 = "value1"  # some comment
    # another comment
```

## Examples

Check out **[Format Tests](./tests/test_format.cpp)** and **[Parser Tests](./tests/test_parser.cpp)**
