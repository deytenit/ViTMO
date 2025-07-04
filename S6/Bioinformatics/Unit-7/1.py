A_PP = 0.9
A_PNP = 0.1
A_NPP = 0.2
A_NPNP = 0.8

E_PROMOTER = {'A': 0.1, 'C': 0.4, 'G': 0.4, 'T': 0.1}
E_NON_PROMOTER = {'A': 0.3, 'C': 0.2, 'G': 0.2, 'T': 0.3}

def hmm_sequence_probability(sequence):
    pi_p = 0.5
    pi_np = 0.5

    alpha_p = pi_p * E_PROMOTER[sequence[0]]
    alpha_np = pi_np * E_NON_PROMOTER[sequence[0]]

    for nucleotide in sequence[1:]:
        alpha_p_next = (alpha_p * A_PP + alpha_np * A_NPP) * E_PROMOTER[nucleotide]
        alpha_np_next = (alpha_p * A_PNP + alpha_np * A_NPNP) * E_NON_PROMOTER[nucleotide]
        alpha_p, alpha_np = alpha_p_next, alpha_np_next

    return alpha_p + alpha_np


sequence = input()

probability = hmm_sequence_probability(sequence)

print(f"{probability:.10f}")
