#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "se/Searcher.hpp"

namespace {

std::stringstream create_ss(const std::string &str) {
    return std::stringstream(str);
}

template <class... Args>
constexpr std::size_t count_args(Args &&...args) {
    return sizeof...(Args);
}

template <class It>
class SeqPrinter {
    It m_begin;
    It m_end;
    std::string_view m_sep;

public:
    SeqPrinter(const It begin, const It end, const std::string_view sep = ",")
        : m_begin(begin), m_end(end), m_sep(sep) {}

    std::ostream &print(std::ostream &strm) const {
        bool first = true;
        for (auto it = m_begin; it != m_end; ++it) {
            if (!first) {
                strm << m_sep;
            } else {
                first = false;
            }
            strm << *it;
        }
        return strm;
    }

    friend std::ostream &operator<<(std::ostream &strm, const SeqPrinter &sp) { return sp.print(strm); }
};

template <class It>
SeqPrinter<It> sequence_printer(It begin, It end) {
    return {begin, end};
}

}  // namespace

TEST(SearchEngineBasicTests, Singleton) {
    Searcher::Filename singleton_file("singleton.txt");
    auto singleton_stream = create_ss("a_single_word");

    Searcher s;
    s.add_document(singleton_file, singleton_stream);

    {
        const auto [begin, end] = s.search("a");
        EXPECT_EQ(begin, end);
    }
    {
        const auto [begin, end] = s.search("a_single_word");
        ASSERT_NE(begin, end);
        EXPECT_EQ(singleton_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
}

TEST(SearchEngineBasicTests, AddSimpleDocument) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    auto [begin, end] = s.search("engine");
    ASSERT_NE(begin, end);
    EXPECT_EQ(*begin, simple_file);
    begin++;
    EXPECT_EQ(begin, end);
}

TEST(SearchEngineBasicTests, AddEmptyDocument) {
    Searcher::Filename empty_file("test/etc/empty_file.txt");
    std::ifstream empty_file_stream(empty_file);

    Searcher s;
    s.add_document(empty_file, empty_file_stream);
    auto [begin, end] = s.search("engine");

    ASSERT_EQ(begin, end);
}

TEST(SearchEngineBasicTests, AddTwoDocuments) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);

    {
        auto [begin, end] = s.search("engine");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, simple_file);
        begin++;
        ASSERT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("Ishmael");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, call_me_ishmael);
        begin++;
        ASSERT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("Boris");
        ASSERT_EQ(begin, end);
    }
    {
        const auto [begin, end] = s.search("is, to, a, of");
        ASSERT_NE(begin, end);
        EXPECT_EQ(2, std::distance(begin, end));
        EXPECT_EQ(1, std::count(begin, end, simple_file));
        EXPECT_EQ(1, std::count(begin, end, call_me_ishmael));
    }
    {
        const auto [begin, end] = s.search("is, to, A, a, of");
        ASSERT_NE(begin, end);
        EXPECT_EQ(2, std::distance(begin, end));
        EXPECT_EQ(1, std::count(begin, end, simple_file));
        EXPECT_EQ(1, std::count(begin, end, call_me_ishmael));
    }
    {
        const auto [begin, end] = s.search("is, to, A, a, of, from");
        ASSERT_NE(begin, end);
        EXPECT_EQ(call_me_ishmael, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
}

TEST(SearchEngineBasicTests, AddRemoveEmptyDocument) {
    Searcher::Filename simple_file("simple.txt");
    auto simple_stream = create_ss("One, two, three.");
    Searcher::Filename empty_file("empty.txt");
    auto empty_stream = create_ss("   ...- ");

    Searcher s;
    s.add_document(simple_file, simple_stream);
    s.add_document(empty_file, empty_stream);

    {
        const auto [begin, end] = s.search("two");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
    {
        const auto [begin, end] = s.search("three One");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }

    s.remove_document(empty_file);

    {
        const auto [begin, end] = s.search("two");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
    {
        const auto [begin, end] = s.search("three");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
}

TEST(SearchEngineBasicTests, AddRemoveSingleton) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename singleton_file("singleton.txt");
    auto singleton_stream = create_ss("Aleph");

    Searcher s;
    s.add_document(simple_file, simple_file_stream);

    {
        const auto [begin, end] = s.search("A");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
    {
        const auto [begin, end] = s.search("query");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }

    s.add_document(singleton_file, singleton_stream);

    {
        const auto [begin, end] = s.search("A");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
    {
        const auto [begin, end] = s.search("query");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
    {
        const auto [begin, end] = s.search("Aleph");
        ASSERT_NE(begin, end);
        EXPECT_EQ(singleton_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }

    s.remove_document(singleton_file);

    {
        const auto [begin, end] = s.search("A");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
    {
        const auto [begin, end] = s.search("speed");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
    {
        const auto [begin, end] = s.search("Aleph");
        EXPECT_EQ(begin, end);
    }
}

TEST(SearchEngineBasicTests, AddRemoveRepeatingSingleton) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename singleton_file("singleton.txt");
    auto singleton_stream = create_ss("engine");

    Searcher s;
    s.add_document(simple_file, simple_file_stream);

    {
        const auto [begin, end] = s.search("engine");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
    {
        const auto [begin, end] = s.search("engine is");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }

    s.add_document(singleton_file, singleton_stream);

    {
        const auto [begin, end] = s.search("engine");
        ASSERT_NE(begin, end);
        EXPECT_EQ(2, std::distance(begin, end));
        EXPECT_EQ(1, std::count(begin, end, simple_file));
        EXPECT_EQ(1, std::count(begin, end, singleton_file));
    }
    {
        const auto [begin, end] = s.search("engine is");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }

    s.remove_document(singleton_file);

    {
        const auto [begin, end] = s.search("engine");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
    {
        const auto [begin, end] = s.search("engine is");
        ASSERT_NE(begin, end);
        EXPECT_EQ(simple_file, *begin);
        EXPECT_EQ(1, std::distance(begin, end));
    }
}

TEST(SearchEngineBasicTests, PunctBeforeAndAfter) {
    Searcher::Filename punct_before_and_after("test/etc/punct_before_and_after.txt");
    std::ifstream punct_before_and_after_stream(punct_before_and_after);

    Searcher s;
    s.add_document(punct_before_and_after, punct_before_and_after_stream);

    {
        auto [begin, end] = s.search("Ishmael");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, punct_before_and_after);
        begin++;
        ASSERT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("Boris");
        ASSERT_EQ(begin, end);
    }
}

TEST(SearchEngineBasicTests, PunctInTheMiddle) {
    Searcher::Filename punct_in_the_middle("test/etc/punct_in_the_middle.txt");
    std::ifstream punct_in_the_middle_stream(punct_in_the_middle);

    Searcher s;
    s.add_document(punct_in_the_middle, punct_in_the_middle_stream);

    {
        auto [begin, end] = s.search("Is-hma--el");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, punct_in_the_middle);
        begin++;
        ASSERT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("Ishmael");
        ASSERT_EQ(begin, end);
    }
}

TEST(SearchEngineBasicTests, TextWithoutSpaceSymbols) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);
    Searcher::Filename text_without_space_symbols("test/etc/text_without_space_symbols.txt");
    std::ifstream text_without_space_symbols_stream(text_without_space_symbols);

    Searcher s;
    s.add_document(text_without_space_symbols, text_without_space_symbols_stream);
    {
        auto [begin, end] = s.search("CallmeIshmael");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, text_without_space_symbols);
        begin++;
        ASSERT_EQ(begin, end);
    }

    s.add_document(simple_file, simple_file_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);
    {
        auto [begin, end] = s.search("engine");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, simple_file);
        begin++;
        ASSERT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("Ishmael");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, call_me_ishmael);
        begin++;
        ASSERT_EQ(begin, end);
    }
}

TEST(SearchEngineBasicTests, ThreePairsOfIterators) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);

    auto [begin_first, end_first]   = s.search("engine");
    auto [begin_second, end_second] = s.search("Ishmael");
    auto [begin_third, end_third]   = s.search("Boris");

    ASSERT_NE(begin_first, end_first);
    ASSERT_EQ(*begin_first, simple_file);
    begin_first++;
    ASSERT_EQ(begin_first, end_first);

    ASSERT_NE(begin_second, end_second);
    ASSERT_EQ(*begin_second, call_me_ishmael);
    begin_second++;
    ASSERT_EQ(begin_second, end_second);

    ASSERT_EQ(begin_third, end_third);
}

TEST(SearchEngineBasicTests, WordInTheTwoDocuments) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);

    auto [begin, end] = s.search("the");
    ASSERT_NE(begin, end);
    ASSERT_TRUE(*begin == simple_file || *begin == call_me_ishmael);
    begin++;
    ASSERT_NE(begin, end);
    ASSERT_TRUE(*begin == simple_file || *begin == call_me_ishmael);
    begin++;
    ASSERT_EQ(begin, end);
}

TEST(SearchEngineBasicTests, NoDocumentsInSearcher) {
    Searcher s;

    auto [begin, end] = s.search("the");
    ASSERT_EQ(begin, end);
}

TEST(SearchEngineBasicTests, ExtraSpacesAndPunctInFile) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename extra_spaces_and_punct("test/etc/extra_spaces_and_punct.txt");
    std::ifstream extra_spaces_and_punct_stream(extra_spaces_and_punct);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    s.add_document(extra_spaces_and_punct, extra_spaces_and_punct_stream);

    {
        auto [begin, end] = s.search("the");
        ASSERT_NE(begin, end);
        EXPECT_TRUE(*begin == simple_file || *begin == extra_spaces_and_punct);
        begin++;
        ASSERT_NE(begin, end);
        EXPECT_TRUE(*begin == simple_file || *begin == extra_spaces_and_punct);
        begin++;
        EXPECT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("\"the speed\"");
        ASSERT_NE(begin, end);
        EXPECT_TRUE(*begin == simple_file || *begin == extra_spaces_and_punct);
        begin++;
        ASSERT_NE(begin, end);
        EXPECT_TRUE(*begin == simple_file || *begin == extra_spaces_and_punct);
        begin++;
        EXPECT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("\"the speed of query\"");
        EXPECT_EQ(begin, end);
    }
}

TEST(SearchEngineBasicTests, AddSameDocumentTwice) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    std::ifstream simple_file_stream2(simple_file);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);
    s.add_document(simple_file, simple_file_stream2);

    {
        auto [begin, end] = s.search("engine");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, simple_file);
        begin++;
        ASSERT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("Boris");
        ASSERT_EQ(begin, end);
    }
}

TEST(SearchQueryTests, SingleWordInAPhrase) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    auto [begin, end] = s.search("\"a\"");
    ASSERT_NE(begin, end);
    EXPECT_EQ(*begin, simple_file);
    begin++;
    EXPECT_EQ(begin, end);
}

TEST(SearchQueryTests, TwoWordsInTheQuery) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);

    {
        auto [begin, end] = s.search("the city");
        ASSERT_NE(begin, end);
        EXPECT_EQ(*begin, call_me_ishmael);
        begin++;
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("the implementation");
        ASSERT_NE(begin, end);
        EXPECT_EQ(*begin, simple_file);
        begin++;
        EXPECT_EQ(begin, end);
    }
}

TEST(SearchQueryTests, TwoWordsWithQuotesInTheQuery) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename simple_file_copy("test/etc/simple_file_copy.txt");
    std::ifstream simple_file_copy_stream(simple_file_copy);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);

    {
        auto [begin, end] = s.search("\"the city\"");
        ASSERT_NE(begin, end);
        EXPECT_EQ(*begin, call_me_ishmael);
        begin++;
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("\"the implementation\"");
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("\"the query\"");
        ASSERT_NE(begin, end);
        EXPECT_EQ(*begin, simple_file);
        begin++;
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("\"a search\"");
        ASSERT_NE(begin, end);
        EXPECT_EQ(*begin, simple_file);
        begin++;
        EXPECT_EQ(begin, end);
    }

    s.add_document(simple_file_copy, simple_file_copy_stream);
    {
        auto [begin, end] = s.search("\"the query\"");
        ASSERT_NE(begin, end);
        EXPECT_TRUE(*begin == simple_file || *begin == simple_file_copy);
        begin++;
        ASSERT_NE(begin, end);
        EXPECT_TRUE(*begin == simple_file || *begin == simple_file_copy);
        begin++;
        EXPECT_EQ(begin, end);
    }
}

TEST(SearchQueryTests, PhraseAndIndividualWords) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.add_document(call_me_ishmael, call_me_ishmael_stream);
    s.add_document(simple_file, simple_file_stream);

    {
        auto [begin, end] = s.search("\"a\" engine");
        ASSERT_NE(begin, end);
        EXPECT_EQ(*begin, simple_file);
        begin++;
        EXPECT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("\"a search\" engine");
        ASSERT_NE(begin, end);
        EXPECT_EQ(*begin, simple_file);
        begin++;
        EXPECT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("optimize \"a search\" query \"the speed\"");
        ASSERT_NE(begin, end);
        EXPECT_EQ(*begin, simple_file);
        begin++;
        EXPECT_EQ(begin, end);
    }
}

TEST(SearchQueryTests, ComplexQuery) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename simple_file_copy("test/etc/simple_file_copy.txt");
    std::ifstream simple_file_copy_stream(simple_file_copy);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    s.add_document(simple_file_copy, simple_file_copy_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);

    {
        auto [begin, end] = s.search("\"the city\" \"Call me Ishmael\" Manhattoes \"Corlears Hook\" wharves");
        ASSERT_NE(begin, end);
        EXPECT_EQ(*begin, call_me_ishmael);
        begin++;
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("the implementation of Ishmael");
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("\"the implementation of Ishmael\"");
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("\"London of a search engine\"");
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("\"Call me BUGAGA Ishmael\"");
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("a goal \"a goal of a\" search engine");
        ASSERT_NE(begin, end);
        EXPECT_EQ(2, std::distance(begin, end));
        EXPECT_EQ(1, std::count(begin, end, simple_file));
        EXPECT_EQ(1, std::count(begin, end, simple_file_copy));
    }

    {
        auto [begin, end] = s.search("\"the query\" \"A goal\" the the the the the optimize");
        ASSERT_NE(begin, end);
        EXPECT_TRUE(*begin == simple_file || *begin == simple_file_copy);
        begin++;
        EXPECT_NE(begin, end);
        EXPECT_TRUE(*begin == simple_file || *begin == simple_file_copy);
        begin++;
        EXPECT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("\"the query\" \"The goal\" the the the the the optimize");
        EXPECT_EQ(begin, end);
    }
}

TEST(SearchQueryTests, IncorrectQuery) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);

    ASSERT_THROW(s.search(""), Searcher::BadQuery);
    ASSERT_THROW(s.search(" "), Searcher::BadQuery);
    ASSERT_THROW(s.search(" \"the query"), Searcher::BadQuery);
    ASSERT_THROW(s.search(" the query\""), Searcher::BadQuery);
    ASSERT_THROW(s.search("\"\""), Searcher::BadQuery);
    ASSERT_THROW(s.search(" ,_\"\""), Searcher::BadQuery);
    ASSERT_THROW(s.search("\"...\""), Searcher::BadQuery);
    ASSERT_THROW(s.search("\"\":  "), Searcher::BadQuery);
    ASSERT_THROW(s.search("(_*_)"), Searcher::BadQuery);
}

TEST(SearchEngineRemoveDocumentTests, RemoveDocumentSimpleTest) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);

    Searcher s;

    s.add_document(simple_file, simple_file_stream);
    {
        auto [begin, end] = s.search("engine");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, simple_file);
        begin++;
        ASSERT_EQ(begin, end);
    }

    s.remove_document(simple_file);
    {
        auto [begin, end] = s.search("engine");
        ASSERT_EQ(begin, end);
    }
}

TEST(SearchEngineRemoveDocumentTests, RemoveSeveralDocuments) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename simple_file_copy("test/etc/simple_file_copy.txt");
    std::ifstream simple_file_copy_stream(simple_file_copy);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.add_document(simple_file, simple_file_stream);
    s.add_document(simple_file_copy, simple_file_copy_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);

    s.remove_document(simple_file_copy);
    {
        auto [begin, end] = s.search("engine");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, simple_file);
        begin++;
        ASSERT_EQ(begin, end);
    }

    s.remove_document(simple_file);
    {
        auto [begin, end] = s.search("engine");
        ASSERT_EQ(begin, end);
    }

    {
        auto [begin, end] = s.search("Ishmael");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, call_me_ishmael);
        begin++;
        ASSERT_EQ(begin, end);
    }

    s.remove_document(call_me_ishmael);
    {
        auto [begin, end] = s.search("Ishmael");
        ASSERT_EQ(begin, end);
    }
}

TEST(SearchEngineRemoveDocumentTests, IncorrectRemoveCall) {
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;
    s.remove_document(simple_file);
    s.search("engine");

    s.add_document(simple_file, simple_file_stream);
    s.remove_document(call_me_ishmael);
    {
        auto [begin, end] = s.search("engine");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, simple_file);
        begin++;
        ASSERT_EQ(begin, end);
    }

    s.add_document(call_me_ishmael, call_me_ishmael_stream);
    s.remove_document(simple_file);
    {
        auto [begin, end] = s.search("engine");
        ASSERT_EQ(begin, end);
    }
    {
        auto [begin, end] = s.search("Ishmael");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, call_me_ishmael);
        begin++;
        ASSERT_EQ(begin, end);
    }

    s.remove_document(simple_file);
    {
        auto [begin, end] = s.search("Ishmael");
        ASSERT_NE(begin, end);
        ASSERT_EQ(*begin, call_me_ishmael);
        begin++;
        ASSERT_EQ(begin, end);
    }
}

TEST(SearchEngineRemoveDocumentTests, RemoveEmptyDocument) {
    Searcher::Filename empty_file("test/etc/empty_file.txt");
    std::ifstream empty_file_stream(empty_file);
    Searcher::Filename simple_file("test/etc/simple_file.txt");
    std::ifstream simple_file_stream(simple_file);
    Searcher::Filename call_me_ishmael("test/etc/call_me_ishmael.txt");
    std::ifstream call_me_ishmael_stream(call_me_ishmael);

    Searcher s;

    s.add_document(simple_file, simple_file_stream);
    s.add_document(call_me_ishmael, call_me_ishmael_stream);
    {
        auto [begin, end] = s.search("the");
        ASSERT_NE(begin, end);
        ASSERT_TRUE(*begin == simple_file || *begin == call_me_ishmael);
        begin++;
        ASSERT_NE(begin, end);
        ASSERT_TRUE(*begin == simple_file || *begin == call_me_ishmael);
        begin++;
        ASSERT_EQ(begin, end);
    }

    s.add_document(empty_file, empty_file_stream);
    {
        auto [begin, end] = s.search("the");
        ASSERT_NE(begin, end);
        ASSERT_TRUE(*begin == simple_file || *begin == call_me_ishmael);
        begin++;
        ASSERT_NE(begin, end);
        ASSERT_TRUE(*begin == simple_file || *begin == call_me_ishmael);
        begin++;
        ASSERT_EQ(begin, end);
    }

    s.remove_document(empty_file);
    {
        auto [begin, end] = s.search("the");
        ASSERT_NE(begin, end);
        ASSERT_TRUE(*begin == simple_file || *begin == call_me_ishmael);
        begin++;
        ASSERT_NE(begin, end);
        ASSERT_TRUE(*begin == simple_file || *begin == call_me_ishmael);
        begin++;
        ASSERT_EQ(begin, end);
    }
}

TEST(SearchEngineBigDocs, add_and_remove) {
    Searcher::Filename Frankenstein("test/etc/Frankenstein.txt");
    std::ifstream Frankenstein_stream(Frankenstein);
    Searcher::Filename Pride_and_Prejudice("test/etc/Pride_and_Prejudice.txt");
    std::ifstream Pride_and_Prejudice_stream(Pride_and_Prejudice);
    Searcher::Filename The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde(
        "test/etc/The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde.txt");
    std::ifstream The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde_stream(The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde);
    Searcher::Filename Moby_Dick("test/etc/Moby_Dick.txt");
    std::ifstream Moby_Dick_stream(Moby_Dick);
    Searcher::Filename Alices_Adventures_in_Wonderland("test/etc/Alices_Adventures_in_Wonderland.txt");
    std::ifstream Alices_Adventures_in_Wonderland_stream(Alices_Adventures_in_Wonderland);

#define CHECK(query, ...)                                                                                             \
    do {                                                                                                              \
        const auto [begin, end] = s.search(query);                                                                    \
        for (const auto &doc : {__VA_ARGS__}) {                                                                       \
            EXPECT_EQ(1, std::count(begin, end, doc)) << doc;                                                         \
        }                                                                                                             \
        EXPECT_EQ(count_args(__VA_ARGS__), std::distance(begin, end)) << "Found in " << sequence_printer(begin, end); \
    } while (false)
#define NOT_FOUND(query)                                                      \
    do {                                                                      \
        const auto [begin, end] = s.search(query);                            \
        EXPECT_EQ(begin, end) << "Found in " << sequence_printer(begin, end); \
    } while (false)
    Searcher s;
    s.add_document(Frankenstein, Frankenstein_stream);
    s.add_document(Pride_and_Prejudice, Pride_and_Prejudice_stream);
    s.add_document(The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde, The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde_stream);
    s.add_document(Moby_Dick, Moby_Dick_stream);
    s.add_document(Alices_Adventures_in_Wonderland, Alices_Adventures_in_Wonderland_stream);

    CHECK("Prejudice", Frankenstein, Pride_and_Prejudice);
    CHECK("Frankenstein", Frankenstein);
    CHECK("daughters", Pride_and_Prejudice, Moby_Dick);
    CHECK("\"the horses\"", Frankenstein, Pride_and_Prejudice);
    CHECK("\"the horses\" three months", Frankenstein, Pride_and_Prejudice);
    CHECK("\"the horses\" \"three months\"", Frankenstein, Pride_and_Prejudice);
    CHECK("\"the horses\" \"three months\" to know what he had to", Frankenstein, Pride_and_Prejudice);
    CHECK("\"the horses\" \"three months\" \"to know what he had to\"", Pride_and_Prejudice);
    CHECK("a to the and an of the I in that when was his whom you this where what which he she it",
          Alices_Adventures_in_Wonderland, Frankenstein, Moby_Dick, Pride_and_Prejudice,
          The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde);
    CHECK("a to the and an of the I in that when was his whom you this where won what which he she it",
          Alices_Adventures_in_Wonderland, Frankenstein, Pride_and_Prejudice);
    CHECK("\"to order the horses\"", Frankenstein);
    CHECK("\"a subject for\"", Frankenstein);
    CHECK("\"I could\"", Frankenstein, Pride_and_Prejudice, The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde, Moby_Dick,
          Alices_Adventures_in_Wonderland);
    CHECK("\"At first\"", Alices_Adventures_in_Wonderland, Frankenstein, Moby_Dick, Pride_and_Prejudice,
          The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde);
    CHECK("\"my brother No one\"", Frankenstein);
    CHECK(
        "\"Volunteers and financial support to provide volunteers with the assistance they need, are critical to "
        "reaching Project Gutenberg-tm's\"",
        Pride_and_Prejudice, Moby_Dick, Alices_Adventures_in_Wonderland);

    s.remove_document(Frankenstein);
    s.remove_document(Alices_Adventures_in_Wonderland);
    s.remove_document(Moby_Dick);

    CHECK("daughters", Pride_and_Prejudice);
    NOT_FOUND("ksfhiwefhliwehapoheioi");
    NOT_FOUND("\"to order the horses\"");
    NOT_FOUND("\"a subject for\"");
    CHECK("\"I could\"", Pride_and_Prejudice, The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde);
    CHECK("\"At first\"", Pride_and_Prejudice, The_Strange_Case_Of_Dr_Jekyll_And_Mr_Hyde);
    NOT_FOUND("\"my brother No one\"");
    CHECK(
        "\"Volunteers and financial support to provide volunteers with the assistance they need, are critical to "
        "reaching Project Gutenberg-tm's\"",
        Pride_and_Prejudice);
}
