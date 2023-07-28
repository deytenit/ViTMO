import csv
import argparse
from imageio import imread


def generate_csv_from_img(in_filename, out_filename):
    image = imread(in_filename)
    height, width, dimension = image.shape
    with open(out_filename, 'w') as csv_file:
        image_writer = csv.writer(csv_file, delimiter=' ')
        image_writer.writerow([width, height])
        for i in range(width):
            for j in range(height):
                r, g, b = image[j, i, :]
                image_writer.writerow([r, g, b])

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generates image from input csv file.')
    parser.add_argument('input_filename', metavar='input', type=str, help='Filename or local filepath to an image')
    parser.add_argument('output_filename', metavar='output', type=str, help='Filename of local filepath to output csv')
    args = parser.parse_args()
    generate_csv_from_img(args.input_filename, args.output_filename)
