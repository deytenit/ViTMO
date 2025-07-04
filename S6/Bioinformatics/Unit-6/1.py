MASSES = {
    'A': 71,  'C': 103, 'D': 115, 'E': 129, 'F': 147,
    'G': 57,  'H': 137, 'I': 113, 'K': 128, 'L': 113,
    'M': 131, 'N': 114, 'P': 97,  'Q': 128, 'R': 156,
    'S': 87,  'T': 101, 'V': 99,  'W': 186, 'Y': 163
}

peptide = input()

spectrum = set()
spectrum.add(0)

prefix_sum = 0
for aa in peptide:
    prefix_sum += MASSES[aa]
    spectrum.add(prefix_sum)

suffix_sum = 0
for aa in reversed(peptide):
    suffix_sum += MASSES[aa]
    spectrum.add(suffix_sum)

sorted_spectrum = sorted(spectrum)

print(' '.join(map(str, sorted_spectrum)))
