#ifndef SE_SEARCHER_HPP
#define SE_SEARCHER_HPP

#include <filesystem>

class Searcher {
public:
    using Filename = std::filesystem::path;

    // index modification
    void add_document(const Filename& filename, std::istream& strm);

    void remove_document(const Filename& filename);

    // queries
    class DocIterator {
    public:
        using value_type = const Filename;
        //...
    };

    class BadQuery: public std::exception {
        //...
    };

    std::pair<DocIterator, DocIterator> search(const std::string& query) const;
};

#endif  // SE_SEARCHER_HPP
