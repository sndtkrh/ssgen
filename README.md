# ssgen
ssgen is a starry sky generator.
ssgen calculates positions of stars and generates images of starry sky.
You can get a starry sky of any place and time.

## Requirements
- `g++ 4.7 or more`
- `Python3 or more`
  -`Pillow` to generate `.png` image.

## How to build
```$ make```

## How to get starry sky
Firstly, you will get position data of stars as follow:
```$ ./ssgen -o stars.txt```
Secondary, you can get starry sky image:
```$ python makeimage.py```

## Licence
MIT Licence
