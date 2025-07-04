MASSES = {
    'A': 71,  'C': 103, 'D': 115, 'E': 129, 'F': 147,
    'G': 57,  'H': 137, 'I': 113, 'K': 128, 'L': 113,
    'M': 131, 'N': 114, 'P': 97,  'Q': 128, 'R': 156,
    'S': 87,  'T': 101, 'V': 99,  'W': 186, 'Y': 163
}

ACIDS = {
    71: 'A', 103: 'C', 115: 'D', 129: 'E', 147: 'F',
    57: 'G', 137: 'H', 113: 'I', 128: 'K', 131: 'M',  # Choosing I over L and K over Q.
    114: 'N', 97: 'P', 156: 'R', 87: 'S', 101: 'T',
    99: 'V', 186: 'W', 163: 'Y'
}

class Node:
    def __init__(self, weight):
        self.weight = weight
        self.edges = []

spectrum = list(map(int, input().split()))
total_mass = spectrum[-1]

graph = [Node(w) for w in spectrum]

for j in range(len(graph)):
    for i in range(j - 1, -1, -1):
        delta = graph[j].weight - graph[i].weight
        if delta in ACIDS:
            letter = ACIDS[delta]
            graph[i].edges.append((letter, graph[j]))

def get_spectrum(peptide):
    prefix_masses = [0]
    for aa in peptide:
        prefix_masses.append(prefix_masses[-1] + MASSES[aa])
    suffix_masses = [0]
    for aa in reversed(peptide):
        suffix_masses.append(suffix_masses[-1] + MASSES[aa])
    spec = sorted(set(prefix_masses + suffix_masses))
    return spec

def check_spectrum(peptide):
    return get_spectrum(peptide) == spectrum

def dfs(vertex, seq):
    if not vertex.edges:
        if vertex.weight == total_mass and check_spectrum(seq):
            return seq
        return ''

    for letter, next_vertex in vertex.edges:
        res = dfs(next_vertex, seq + letter)
        if res:
            return res

    return ''

print(dfs(graph[0], ''))

