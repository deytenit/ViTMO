def solve():
    M = int(input())

    f_outputs = []
    num_truth_table_entries = 1 << M
    for _ in range(num_truth_table_entries):
        f_outputs.append(int(input()))

    true_minterm_patterns = []
    false_minterm_patterns = []

    for i in range(num_truth_table_entries):
        pattern_str = format(i, f'0{M}b') if M > 0 else ''
        pattern = tuple(int(bit) for bit in pattern_str)

        if f_outputs[i] == 1:
            true_minterm_patterns.append(pattern)
        else:
            false_minterm_patterns.append(pattern)

    num_true_minterms = len(true_minterm_patterns)
    num_false_minterms = len(false_minterm_patterns)

    if num_true_minterms == 0:
        print(1)
        print(1)

        weights_parts = ['0.0'] * M
        bias_str = '-0.5'
        if M == 0:
             print(bias_str)
        else:
             print(f'{" ".join(weights_parts)} {bias_str}')
        return

    if num_false_minterms == 0:
        print(1)
        print(1)

        weights_parts = ['0.0'] * M
        bias_str = '0.5'
        if M == 0:
            print(bias_str)
        else:
            print(f'{' '.join(weights_parts)} {bias_str}')
        return

    print(2)

    minterms_to_implement_in_l1 = true_minterm_patterns
    use_or_for_final_layer = True

    if num_true_minterms > 512:
        minterms_to_implement_in_l1 = false_minterm_patterns
        use_or_for_final_layer = False

    n1_neurons_l1 = len(minterms_to_implement_in_l1)

    print(f'{n1_neurons_l1} 1')

    l1_neuron_details_output = []
    for p_pattern_tuple in minterms_to_implement_in_l1:
        current_neuron_weights = []
        sum_of_bits_in_pattern = 0
        if M > 0:
            for bit_in_pattern in p_pattern_tuple:
                current_neuron_weights.append(float(2 * bit_in_pattern - 1))
                sum_of_bits_in_pattern += bit_in_pattern

        current_neuron_bias = -float(sum_of_bits_in_pattern) + 0.5

        weights_str_parts = [f'{w:.1f}' for w in current_neuron_weights]

        if M == 0:
             l1_neuron_details_output.append(f'{current_neuron_bias:.1f}')
        else:
             l1_neuron_details_output.append(f'{' '.join(weights_str_parts)} {current_neuron_bias:.1f}')

    for detail_line in l1_neuron_details_output:
        print(detail_line)

    l2_neuron_input_count = n1_neurons_l1
    l2_neuron_weights_list = []
    l2_neuron_bias_val = 0.0

    if use_or_for_final_layer:
        l2_neuron_weights_list = [1.0] * l2_neuron_input_count
        l2_neuron_bias_val = -0.5
    else:
        l2_neuron_weights_list = [-1.0] * l2_neuron_input_count
        l2_neuron_bias_val = 0.5

    final_weights_str_parts = [f'{w:.1f}' for w in l2_neuron_weights_list]

    print(f'{' '.join(final_weights_str_parts)} {l2_neuron_bias_val:.1f}')

solve()

