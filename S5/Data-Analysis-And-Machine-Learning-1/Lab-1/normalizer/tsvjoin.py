import os

def join_files(file_list, output_file):
    with open(output_file, 'w', encoding='utf-8') as outfile:
        for file_name in file_list:
            if os.path.isfile(file_name):
                with open(file_name, 'r', encoding='utf-8') as infile:
                    outfile.write(infile.read())
            else:
                print(f'{file_name} does not exist')
    print(f'Successfully joined into {output_file}')

FROM = 1;
TO = 501;
STEP = 5;
SRC = "../files/albums_plain";



files = [f'{SRC}[{f},{f + STEP - 1}].tsv' for f in range(FROM, TO, STEP)]

join_files(files, f'{SRC}.tsv')
    