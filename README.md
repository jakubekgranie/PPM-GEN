# PPM-GEN

## Description
Hello there! This program consists of cpp code generating .ppm files based on loop variables and/or random numbers.
The maximum size of the image is 255 x 255 and the minimum is 1 x 1. This can be changed manually, however, have in mind that many programs may struggle to view large .ppm bitmaps.
Hence height and width input will default to 10 if the conditions are violated.

It contains 4 modes:

- 1) Standard. No user input used in color generation is taken and the output depends on loop progression (effectively pixel's position) (RGB: i, j, 0).
- 2) Half-random. Only the B is random, rest depends on loop progression (effectively pixel's position) (RGB: i, j, B. B e <0, 255>).
- 3) Random. All color values are random (RGB: R, G, B. R, G, B e <0, 255>).
- 4) Custom. B is set by the user, remaining color values depend on loop progression (effectively pixel's position) (RGB: i, j, B. B e <0, 255>).

Invalid mode input defaults to the first option.

## Requirements
To run the uncompiled version, one must have a .cpp compiler in order to create an executable.
Also, the program must be allowed to manipulate files in its directory, due to it creating and operating on the 'bitmap.pph' file.
No other entities are interacted with.

## Copyright notice
**(C)** Jacob Namyślak.
*All rights reserved.*
