# PPM-GEN

## Description
Hello there! This program consists of cpp code generating .ppm files based on loop variables and/or random numbers.
The maximum size of the image is 255 x 255 and the minimum is 1 x 1. This can be changed manually, however, have in mind that many programs may struggle to view large .ppm bitmaps.
Hence height and width input will default to 10 if the conditions are violated.

It contains 5 writing modes:

- 1) Standard. No user input used in color generation is taken and the output depends on loop progression (effectively pixel's position) (RGB: i, j, 0);
- 2) Half-random. Only the B is random, rest depends on loop progression (effectively pixel's position) (RGB: i, j, B. B e <0, 255>);
- 3) Random. All color values are random (RGB: R, G, B. R, G, B e <0, 255>);
- 4) Custom. B is set by the user, remaining color values depend on loop progression (effectively pixel's position) (RGB: i, j, B. B e <0, 255>);
- 5) Embed. Accepts user input. Behaves like mode 1, except B's are used as the aforementioned's storage (RGB: i, j, B, B e <0, 1>).

overwrite mode (see mode 5, but much more impactless on the visual aspects), read mode and additional robust encryption utilities to keep your data secure.

Only Polish is supported at the moment.

## Requirements
To run the uncompiled version, one must have a .cpp compiler in order to create an executable.
Also, the program must be allowed to manipulate files in its directory, due to it creating and operating on the 'bitmap.ppm' file, and, optionally, encryption key and log (.txt) files.
No other entities are interacted with.

## Copyright notice
**(C)** Jacob Namyślak, 01/01/2024
*All rights reserved.*
