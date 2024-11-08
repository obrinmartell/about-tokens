#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

// Token types
enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, OPERATOR, PUNCTUATION, WHITESPACE, COMMENT, UNKNOWN
};

// Token structure to hold the token type and its value
struct Token {
    TokenType type;
    string value;
    int line;
    int column;
};

// Regular expressions for token matching
const vector<pair<TokenType, string>> token_specification = {
    {KEYWORD, R"(\b(int|float|char|return|if|else|for|while|void)\b)"},
    {IDENTIFIER, R"(\b[A-Za-z_][A-Za-z0-9_]*\b)"},
    {NUMBER, R"(\b\d+\b)"},
    {OPERATOR, R"([+\-*/=<>!&|]+)"},
    {PUNCTUATION, R"([{}()\[\],;.] )"},
    {WHITESPACE, R"(\s+)"},
    {COMMENT, R"(//.*|/\*.*?\*/)"},
};

// Function to get the token type from regex pattern
TokenType getTokenType(const string& str) {
    for (const auto& spec : token_specification) {
        regex token_regex(spec.second);
        if (regex_match(str, token_regex)) {
            return spec.first;
        }
    }
    return UNKNOWN;
}

// Function to scan the C code and tokenize it
void scanCode(const string& code) {
    int line_num = 1;
    int column_start = 0;

    string::const_iterator start = code.begin();
    while (start != code.end()) {
        string::const_iterator end = start;

        // Try matching each token specification
        bool matched = false;
        for (const auto& spec : token_specification) {
            regex token_regex(spec.second);
            smatch match;
            if (regex_search(start, code.end(), match, token_regex, regex_constants::match_continuous)) {
                matched = true;
                string token_value = match.str(0);

                Token token;
                token.type = spec.first;
                token.value = token_value;
                token.line = line_num;
                token.column = distance(code.begin(), start) - column_start;

                // Skip over matched part
                start += token_value.size();

                // Print token details
                if (token.type != WHITESPACE && token.type != COMMENT) {
                    cout << "Line " << token.line << ", Column " << token.column
                        << ": " << (token.type == KEYWORD ? "KEYWORD" : token.type == IDENTIFIER ? "IDENTIFIER" :
                            token.type == NUMBER ? "NUMBER" : token.type == OPERATOR ? "OPERATOR" :
                            token.type == PUNCTUATION ? "PUNCTUATION" : "UNKNOWN")
                        << " -> " << token.value << endl;
                }
                break;
            }
        }

        // Handle unknown tokens
        if (!matched) {
            cout << "Unknown token: " << *start << endl;
            start++;
        }
    }
}

int main() {
    // C code to be scanned
    string code = R"(
    int main() {
        int a = 10;
        float b = 20.5;
        // This is a comment
        return a + b;
    }
    )";

    scanCode(code);

    return 0;
}