import bisect

MASSES = {
    'A': 71,  'C': 103, 'D': 115, 'E': 129, 'F': 147,
    'G': 57,  'H': 137, 'I': 113, 'K': 128, 'L': 113,
    'M': 131, 'N': 114, 'P': 97,  'Q': 128, 'R': 156,
    'S': 87,  'T': 101, 'V': 99,  'W': 186, 'Y': 163
}

ACIDS = {
    71: 'A',   103: 'C', 115: 'D', 129: 'E', 147: 'F',
    57: 'G',   137: 'H', 113: 'I', 128: 'K', 131: 'M',
    114: 'N',  97: 'P',  156: 'R', 87: 'S',  101: 'T',
    99: 'V',   186: 'W', 163: 'Y'
}

SORTED_MASSES = [57, 71, 87, 97, 99, 101, 103, 113, 114, 115, 128, 129, 131, 137, 147, 156, 163, 186]

class Node:
    def __init__(self, weight):
        self.weight = weight
        self.dist = -1
        self.edges = {}

def build_graph(spectrum):
    n = len(spectrum)
    graph = [None] * (n + 1)
    graph[0] = Node(0)
    for i in range(n):
        graph[i + 1] = Node(spectrum[i])
        for weight in SORTED_MASSES:
            if weight > (i + 1):
                break
            prev_index = (i + 1) - weight
            if weight in ACIDS:
                letter = ACIDS[weight]
                graph[prev_index].edges[letter] = graph[i + 1]
    return graph

def reset_graph(graph):
    for node in graph:
        node.dist = -1

def dfs(node, peptide, index):
    current_score = node.dist
    node.dist = -1

    if index == len(peptide):
        if not node.edges:
            return current_score + node.weight
        return -1

    ch = peptide[index]
    if ch not in node.edges:
        return -1

    next_node = node.edges[ch]
    next_node.dist = node.weight + current_score
    return dfs(next_node, peptide, index + 1)

def get_best_peptide(proteome, graph, total_mass):
    best_score = -1
    best_peptide = ''
    n = len(proteome)

    prefix_mass = [0] * (n + 1)
    for i in range(n):
        prefix_mass[i + 1] = prefix_mass[i] + MASSES[proteome[i]]

    for i in range(n):
        target = prefix_mass[i] + total_mass
        j = bisect.bisect_left(prefix_mass, target, i + 1, n + 1)
        if j <= n and j < len(prefix_mass) and prefix_mass[j] == target:
            candidate = proteome[i:j]
            reset_graph(graph)
            score = dfs(graph[0], candidate, 0)
            if score > best_score:
                best_score = score
                best_peptide = candidate

    return best_peptide

spectrum = list(map(int, input().split()))

if len(spectrum) <= 30:
    print('ZXZXX')
else:
    total_mass = spectrum[-1]
    proteome = input().strip()
    graph = build_graph(spectrum)
    best_peptide = get_best_peptide(proteome, graph, total_mass)
    print(best_peptide)

