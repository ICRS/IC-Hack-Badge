cd build && make -j4
cd ..

python scripts/build.py
#picotool load build/data_2.bin -o 10040000 -f
#picotool load build/data.bin -o 10040100 -f
#picotool load build/temp.bin -fx
picotool uf2 convert build/temp.bin build/out.uf2
