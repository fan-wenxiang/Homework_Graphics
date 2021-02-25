#!/usr/bin/env bash

# If project not ready, generate cmake file.
if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

# Build project.
cd build
make -j
cd ..

# Run all testcases. 
# You can comment some lines to disable the run of specific examples.
mkdir -p output
#第二个参数为输入的场景，第三个参数为生成的图片，格式为tga或bmp，第四个参数为samples的取值，最终取样数为4*samples
#若有第五、六个参数，则表示读入一张之前生成的图片继续增加其samples数，第五个参数为读入的图片，格式只能为tga，第六个参数为所读入图片的samples值
bin/PA1 testcases/scene01_pt.txt output/scene01.bmp 5000 testcases/scene01.tga 5000
#bin/PA1 testcases/scene02_mesh.txt output/scene02.bmp 500 testcases/scene02.tga 500
#bin/PA1 testcases/scene03_depth.txt output/scene03.bmp 1000 testcases/scene03.tga 1000
