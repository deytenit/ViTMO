#ifndef GA_GENOME_HPP
#define GA_GENOME_HPP

#include <string>
#include <vector>

namespace genome {
std::string assembly(size_t k, const std::vector<std::string>& reads);
}  // namespace genome

#endif  // GA_GENOME_HPP
