import numpy as np

def print_matrix(matrix):
    if matrix is None:
        return
    for row_idx in range(matrix.shape[0]):
        formatted_elements = []
        for col_idx in range(matrix.shape[1]):
            val = matrix[row_idx, col_idx]

            if val == 0.0:
                formatted_elements.append(f"{0.0:.4f}")
            else:
                formatted_elements.append(f"{val:.4f}")

        print(" ".join(formatted_elements))

def solve():
    N, M, K = map(int, input().split())

    operations = []
    for _ in range(N):
        line = input().split()
        op_type = line[0]
        if op_type == 'var':
            operations.append(['var', int(line[1]), int(line[2])])
        elif op_type == 'tnh':
            operations.append(['tnh', int(line[1])])
        elif op_type == 'rlu':
            operations.append(['rlu', int(line[1]), int(line[2])])
        elif op_type == 'mul':
            operations.append(['mul', int(line[1]), int(line[2])])
        elif op_type == 'sum' or op_type == 'had':
            parsed_line = [op_type] + [int(x) for x in line[1:]]
            operations.append(parsed_line)

    node_values = [None] * N

    for i in range(N):
        op_config = operations[i]
        op_type = op_config[0]
        if op_type == 'var':
            r_dim, c_dim = op_config[1], op_config[2]
            matrix_rows_data = []
            for _ in range(r_dim):
                matrix_rows_data.append(list(map(int, input().split())))
            node_values[i] = np.array(matrix_rows_data, dtype=float)
        elif op_type == 'tnh':
            x_node_idx = op_config[1] - 1
            node_values[i] = np.tanh(node_values[x_node_idx])
        elif op_type == 'rlu':
            alpha_inv_val = op_config[1]
            x_node_idx = op_config[2] - 1
            alpha_param = 1.0 / float(alpha_inv_val)
            X_matrix = node_values[x_node_idx]
            result_matrix = np.array(X_matrix, dtype=float)
            result_matrix[X_matrix < 0] *= alpha_param
            node_values[i] = result_matrix
        elif op_type == 'mul':
            a_node_idx = op_config[1] - 1
            b_node_idx = op_config[2] - 1
            node_values[i] = np.dot(node_values[a_node_idx], node_values[b_node_idx])
        elif op_type == 'sum':
            sum_node_indices = [idx - 1 for idx in op_config[2:]]
            current_sum_matrix = np.zeros_like(node_values[sum_node_indices[0]], dtype=float)
            for s_idx in sum_node_indices:
                current_sum_matrix += node_values[s_idx]
            node_values[i] = current_sum_matrix
        elif op_type == 'had':
            had_node_indices = [idx - 1 for idx in op_config[2:]]
            current_prod_matrix = np.array(node_values[had_node_indices[0]], dtype=float)
            for k_idx in range(1, len(had_node_indices)):
                current_prod_matrix *= node_values[had_node_indices[k_idx]]
            node_values[i] = current_prod_matrix

    for j in range(K):
        output_node_idx = N - K + j
        print_matrix(node_values[output_node_idx])

    node_derivatives = [np.zeros_like(val, dtype=float) if val is not None else None for val in node_values]

    for j in range(K):
        output_node_idx = N - K + j
        r_dim, c_dim = node_values[output_node_idx].shape
        derivatives_matrix_rows = []
        for _ in range(r_dim):
            derivatives_matrix_rows.append(list(map(float, input().split())))
        node_derivatives[output_node_idx] = np.array(derivatives_matrix_rows, dtype=float)

    for i in range(N - 1, -1, -1):
        op_config = operations[i]
        op_type = op_config[0]
        current_output_grad = node_derivatives[i]

        if current_output_grad is None:
            continue

        if op_type == 'var':
            pass
        elif op_type == 'tnh':
            x_node_idx = op_config[1] - 1
            Y_val_at_node_i = node_values[i]
            grad_tanh_wrt_X = 1 - Y_val_at_node_i**2
            node_derivatives[x_node_idx] += current_output_grad * grad_tanh_wrt_X
        elif op_type == 'rlu':
            alpha_inv_val = op_config[1]
            x_node_idx = op_config[2] - 1
            alpha_param = 1.0 / float(alpha_inv_val)

            X_val_at_input_node = node_values[x_node_idx]

            grad_prelu_wrt_X = np.ones_like(X_val_at_input_node, dtype=float)
            grad_prelu_wrt_X[X_val_at_input_node < 0] = alpha_param

            node_derivatives[x_node_idx] += current_output_grad * grad_prelu_wrt_X
        elif op_type == 'mul':
            a_node_idx = op_config[1] - 1
            b_node_idx = op_config[2] - 1

            A_val = node_values[a_node_idx]
            B_val = node_values[b_node_idx]

            node_derivatives[a_node_idx] += np.dot(current_output_grad, B_val.T)
            node_derivatives[b_node_idx] += np.dot(A_val.T, current_output_grad)
        elif op_type == 'sum':
            sum_node_indices = [idx - 1 for idx in op_config[2:]]
            for s_idx in sum_node_indices:
                node_derivatives[s_idx] += current_output_grad
        elif op_type == 'had':
            lhs_operand_node_idx = [idx - 1 for idx in op_config[2:]]
            Y_val_at_node_i = node_values[i]

            for k_enum_idx, current_operand_node_idx in enumerate(lhs_operand_node_idx):
                grad_factor_for_operand = np.ones_like(Y_val_at_node_i, dtype=float)

                if len(lhs_operand_node_idx) > 1:
                    for m_enum_idx, rhs_operand_node_idx in enumerate(lhs_operand_node_idx):
                        if m_enum_idx == k_enum_idx:
                            continue
                        grad_factor_for_operand *= node_values[rhs_operand_node_idx]

                node_derivatives[current_operand_node_idx] += current_output_grad * grad_factor_for_operand

    for j in range(M):
        input_node_idx = j
        print_matrix(node_derivatives[input_node_idx])

solve()
