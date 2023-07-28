import pandas as pd
import numpy as np
import argparse
from imageio import imwrite

def generate_img_from_csv(in_filename, out_filename):
    with open(in_filename, 'r') as csv:
        line = csv.readline()
        width, height = [int(item) for item in line.split(' ')]
    df = pd.read_csv(in_filename, sep=' ', skiprows=1, header=None)
    items, dimension = df.shape
    image_data = df.to_numpy().reshape(width, height, dimension)
    image_data = np.transpose(image_data, axes=(1, 0, 2))
    print("Writs image to", out_filename)
    imwrite(out_filename, image_data.astype(np.uint8))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generates image from input csv file.')
    parser.add_argument('input_filename', metavar='input', type=str, help='Filename or local filepath to input csv file')
    parser.add_argument('output_filename', metavar='output', type=str, help='Filename of local filepath to output image')
    args = parser.parse_args()
    generate_img_from_csv(args.input_filename, args.output_filename)