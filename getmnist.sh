#!/bin/bash

mkdir mnist
cd mnist

curl -o 'mnist.zip' 'https://data.deepai.org/mnist.zip'
unzip mnist.zip
rm mnist.zip

gzip -d 'train-images-idx3-ubyte.gz'
gzip -d 'train-labels-idx1-ubyte.gz'
gzip -d 't10k-images-idx3-ubyte.gz'
gzip -d 't10k-labels-idx1-ubyte.gz'
