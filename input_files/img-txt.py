from PIL import Image
import sys

# requires Pillow for python

# arg1 input image as PNG recomment to convert into gray scale prior
# arg2 output file as .txt

im = Image.open(sys.argv[1])

pix = im.load()

num_cols = im.size[0]
num_rows = im.size[1]

file = open(sys.argv[2], "w")

file.write(str(im.size[0]) + " " + str(im.size[1]) + "\n")
for row in range (0, num_rows):
    for col in range (0, num_cols):
        file.write(str(pix[col, row][0]) + " ")
    file.write("\n")

file.close()