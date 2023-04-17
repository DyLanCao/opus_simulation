#!/bin/bash
#文件名: scripts.sh
#用途: convert .ogg to .ogg file

count=1;
for img in `find . -iname '*.bin' `
do
  new=${img%.bin}.pcm

  echo "convert $img to $new"
  ./bin/opus $img $new

done
