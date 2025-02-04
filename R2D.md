# R2D Overview

R2D Commands are in the form:

Command Arg1 Arg2 ... ArgN

## Command Set:

### IMAGE

Takes 2-6 arguments

| Argument | Type | Description |
| --- | --- | --- |
| 1 | int | Width of the image |
| 2 | int | Height of the image |
| 3 | double | Scale of the image |
| 4 | int | X Offset of the image |
| 5 | int | Y Offset of the image |
| 6 | string | Name of the image |

### IOR

Takes at least 1 argument

| Argument | Type | Description |
| --- | --- | --- |
| 1 | int | Number of n Sellmeier coefficients |
| 2...n | double | B coefficients |
| n+1...2n | double | C coefficients |

### MATERIAL

Takes 3-4 arguments

| Argument | Type | Description |
| --- | --- | --- |
| 1 | double | Color R |
| 2 | double | Color G |
| 3 | double | Color B |
| 4 | double | Color A |

### ARC

Takes 5 arguments

| Argument | Type | Description |
| --- | --- | --- |
| 1 | double | Arc origin X |
| 2 | double | Arc origin Y |
| 3 | double | Arc radius |
| 4 | double | Arc start angle |
| 5 | double | Arc end angle |

### LINE

Takes 5 arguments

| Argument | Type | Description |
| --- | --- | --- |
| 1 | double | Line origin X |
| 2 | double | Line origin Y |
| 3 | double | Line direction X |
| 4 | double | Line direction Y |
| 5 | double | Line length |

### RAY

Takes 6 arguments

| Argument | Type | Description |
| --- | --- | --- |
| 1 | double | Ray X |
| 2 | double | Ray Y |
| 3 | double | Ray direction X |
| 4 | double | Ray direction Y |
| 5 | double | Ray wavelength |
| 6 | double | Ray intensity |