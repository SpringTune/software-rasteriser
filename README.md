# software-rasteriser

This repo contains a software rasteriser. It was built for educational purposes, in order to have a better understanding of the graphics pipeline.

## Build
To build this, do not forget to add submodules
```
git clone https://github.com/SpringTune/software-rasteriser.git --recurse-submodules
cd software-rasteriser
mkdir build && cd build
cmake .. && make
./graphics
```

## Features
- Geometry processing (Model->World->Camera->Clip space)
- Orthographic/Perspective projection
- Triangle rasterisation
- Perspective-correct interpolation
- Depth-buffer culling

## Missing features
- Clipping
- Fragment processing

## Important notes
This code is highly unoptimised in favor of clarity. Examples of this include the triangle rasterisation. This implementation uses a per-pixel approach to determine whether a pixel is contained in the triangle, which is used in GPU's due to the parallelisation opportunity.