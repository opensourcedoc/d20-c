# d20

A small console program to demo how to implement DnD (Dragons and Dungeons) style dices in C.

## System Requirements

* A C compiler that implements C99
* GNU Make

## Usage

`d20` default to `1d6`:

```
$ d20
3
```

You may roll different dice by applying different string:

```
$ d20 "1d8+1"
```

Alternatively, roll with command line arguments, like `2d6+2`:

```
$ d20 -r 2 -d 6 -m 2
```

## Copyright

Copyright (c) 2018 Michelle Chen. Licensed under MIT
