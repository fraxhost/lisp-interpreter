#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ==================================== Start: Data Structures ====================================

enum TokenType
{
    // Single-character tokens
    LEFT_PAREN = 0,
    RIGHT_PAREN = 1,
    SINGLE_QUOTE = 2,

    // One or two character tokens

    // Literals
    ATOM_IDENTIFIER = 3,
    ATOM_STRING = 4,
    ATOM_NUMBER = 5,

    // Keywords
    // AND, IF, ELSE

    // End Of File
    TOKEN_EOF = 6
};

union Literal
{
    double number;
    char *string;
};

struct Token
{
    enum TokenType type;   // the kind of token (TokenType)
    char *lexeme;          // actual text
    union Literal literal; // literal value (double or string)
    int line;              // line number
};

struct Scanner
{
    const char *source;   // source code as string
    size_t sourceLength;  // length of source code excluding null character
    struct Token *tokens; // array of tokens
    int tokenCount;       // total number of tokens
    int start;            // start of current lexeme in source
    int current;          // current character in source to look at (not consumed yet)
    int line;             // current line number
};

struct Keyword
{
    const char *name;
    enum TokenType type;
};

struct Keyword keywords[] = {
    // {"and", TOKEN_AND},
    // {"class", TOKEN_CLASS},
    // {"else", TOKEN_ELSE},
    // {"false", TOKEN_FALSE},
    // {"for", TOKEN_FOR},
    // {"fun", TOKEN_FUN},
    // {"if", TOKEN_IF},
    // {"nil", TOKEN_NIL},
    // {"or", TOKEN_OR},
    // {"print", TOKEN_PRINT},
    // {"return", TOKEN_RETURN},
    // {"super", TOKEN_SUPER},
    // {"this", TOKEN_THIS},
    // {"true", TOKEN_TRUE},
    // {"var", TOKEN_VAR},
    // {"while", TOKEN_WHILE}
};

// ====================================== End: Data Structures ======================================

// =================================== Start: Function Definition ===================================

// File Related Functions
char *readFile(const char *path);

// Error Related Functions
void error(int line, const char *message);
void report(int line, const char *where, const char *message);

// Scanner Related Functions
void scanTokens(char *sourceCode);
void scanToken(struct Scanner *scanner);
void addToken(struct Scanner *scanner, enum TokenType tokenType, char *literal);
char peek(int current, int lengthOfSource, const char *sourceCode);
char peekNext(int current, int lengthOfSource, const char *sourceCode);
char advance(struct Scanner *scanner);
bool isAtEnd(int current, int lengthOfSource);
bool isDigit(char c);
bool isAlpha(char c);
bool isAlphaNumeric(char c);
void stringLiteral(struct Scanner *scanner);
void numberLiteral(struct Scanner *scanner);
void identifierOrKeyword(struct Scanner *scanner);

// Utility Functions
enum TokenType getIdentifierType(const char *text);
char *getSubstring(const char *source, int start, int end);

// Print Utility Functions
void printTokens(struct Scanner scanner);

// Run Function
void runFile(const char *path);

// ==================================== End: Function Definition ====================================

// ================================= Start: Function Implementation =================================

void runFile(const char *path)
{
    bool hadError = false;

    printf("----- Running File -----\n\n%s\n\n", path);

    char *sourceCode = readFile(path);
    printf("----- File Contents -----\n\n%s\n\n", sourceCode);

    scanTokens(sourceCode);
}

char *readFile(const char *path)
{
    FILE *file = fopen(path, "rb"); // Open file in binary mode
    if (file == NULL)
    {
        perror("Failed to open file");
        exit(1);
    }

    // Find out file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Allocate buffer to hold the file contents + null terminator
    char *sourceCode = (char *)malloc(fileSize + 1);
    if (sourceCode == NULL)
    {
        perror("Failed to allocate memory");
        fclose(file);
        exit(1);
    }

    // Read the file into the buffer
    size_t bytesRead = fread(sourceCode, 1, fileSize, file);
    if (bytesRead != fileSize)
    {
        perror("Failed to read file completely");
        free(sourceCode);
        fclose(file);
        exit(1);
    }

    // Null-terminate the string
    sourceCode[fileSize] = '\0';

    fclose(file);

    return sourceCode;
}

void error(int line, const char *message)
{
    report(line, "hi", message);
}

void report(int line, const char *where, const char *message)
{
    printf("[line %d] Error %s: %s\n", line, where, message);
}

void scanTokens(char *sourceCode)
{
    struct Scanner scanner =
        {
            .source = sourceCode,
            .sourceLength = strlen(sourceCode), // excluding null terminator
            .tokens = NULL,
            .start = 0,
            .current = 0,
            .line = 1,
        };

    printf("sourceCode: %s\n", scanner.source);
    printf("sourceCode length: %zu\n", scanner.sourceLength);

    while (!isAtEnd(scanner.current, scanner.sourceLength))
    {
        scanner.start = scanner.current;
        scanToken(&scanner);
    }

    // Add EOF token at the end
    // addToken(&scanner, TOKEN_EOF, NULL);

    printTokens(scanner);
}

void scanToken(struct Scanner *scanner)
{
    char currentCharacter = advance(scanner);
    printf("Current Character Index: %d\n", scanner->current);

    switch (currentCharacter)
    {
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        scanner->line++;
        break;
    case '(':
        addToken(scanner, LEFT_PAREN, NULL);
        break;
    case ')':
        addToken(scanner, RIGHT_PAREN, NULL);
        break;
    case '\'':
        addToken(scanner, SINGLE_QUOTE, NULL);
        break;
    case '\"':
        stringLiteral(scanner);
        break;
    default:
        if (isDigit(currentCharacter))
        {
            numberLiteral(scanner);
        }
        else if (isAlpha(currentCharacter))
        {
            identifierOrKeyword(scanner);
        }
        else
        {
            report(scanner->line, &currentCharacter, "Unexpected character");
        }
        break;
    }
}

void addToken(struct Scanner *scanner, enum TokenType tokenType, char *literal)
{
    char *text = getSubstring(scanner->source, scanner->start, scanner->current);

    scanner->tokens = realloc(scanner->tokens, sizeof(struct Token) * (scanner->tokenCount + 1));

    if (!scanner->tokens)
    {
        printf("Failed to realloc tokens\n");
        exit(1);
    }

    struct Token token = {
        .type = tokenType,
        .lexeme = text,
        .line = scanner->line};

    if (tokenType == ATOM_STRING)
    {
        token.literal.string = strdup(literal);
    }
    else if (tokenType == ATOM_NUMBER)
    {
        // convert to double
        double value = strtod(literal, NULL);
        token.literal.number = value;
    }

    scanner->tokens[scanner->tokenCount] = token;
    scanner->tokenCount++;
}

void stringLiteral(struct Scanner *scanner)
{
    // Consume characters until we find closing " or hit end of input
    bool isCurrentCharacterDoubleQuote = peek(scanner->current, scanner->sourceLength, scanner->source) == '\"';
    bool isCurrentCharacterTheEndCharacter = isAtEnd(scanner->current, scanner->sourceLength);

    while (!isCurrentCharacterDoubleQuote && !isCurrentCharacterTheEndCharacter)
    {
        bool isNextCharacterNewLine = peek(scanner->current, scanner->sourceLength, scanner->source) == '\n';

        if (isNextCharacterNewLine)
        {
            scanner->line++; // track newlines
        }

        advance(scanner); // move forward

        isCurrentCharacterDoubleQuote = peek(scanner->current, scanner->sourceLength, scanner->source) == '\"';
        isCurrentCharacterTheEndCharacter = isAtEnd(scanner->current, scanner->sourceLength);
    }

    // If we hit end of source without finding a closing quote
    if (isAtEnd(scanner->current, scanner->sourceLength))
    {
        error(scanner->line, "Unterminated string");
        return;
    }

    advance(scanner); // Consume the closing quote (final ")

    char *literal = getSubstring(scanner->source, scanner->start + 1, scanner->current - 1); // trimming starting and ending quotes

    addToken(scanner, ATOM_STRING, literal);

    free(literal); // prevent memory leak
}

void numberLiteral(struct Scanner *scanner)
{
    // consume integer part
    char currentCharacter = peek(scanner->current, scanner->sourceLength, scanner->source);
    bool isCurrentCharacterDigit = isDigit(currentCharacter);

    while (isCurrentCharacterDigit)
    {
        advance(scanner);

        currentCharacter = peek(scanner->current, scanner->sourceLength, scanner->source);
        isCurrentCharacterDigit = isDigit(currentCharacter);
    }

    // check for fractional part
    currentCharacter = peek(scanner->current, scanner->sourceLength, scanner->source);
    bool isCurrentCharacterCharacterFloatingPoint = currentCharacter == '.';
    char nextCharacter = peekNext(scanner->current, scanner->sourceLength, scanner->source);
    char isNextCharacterDigit = isDigit(nextCharacter);

    if (isCurrentCharacterCharacterFloatingPoint && isNextCharacterDigit)
    {
        advance(scanner); // consume '.'

        currentCharacter = peek(scanner->current, scanner->sourceLength, scanner->source);
        isCurrentCharacterDigit = isDigit(currentCharacter);

        while (isCurrentCharacterDigit)
        {
            advance(scanner);

            currentCharacter = peek(scanner->current, scanner->sourceLength, scanner->source);
            isCurrentCharacterDigit = isDigit(currentCharacter);
        }
    }

    // extract the lexeme
    char *literal = getSubstring(scanner->source, scanner->start, scanner->current);

    // add token
    addToken(scanner, ATOM_NUMBER, literal);

    free(literal); // prevent memory leak
}

void identifierOrKeyword(struct Scanner *scanner)
{
    char currentCharacter = peek(scanner->current, scanner->sourceLength, scanner->source);
    bool isCurrentCharacterAlphaNumeric = isAlphaNumeric(currentCharacter);
    bool isCurrentCharacterTheEndCharacter = isAtEnd(scanner->current, scanner->sourceLength);

    while (!isCurrentCharacterTheEndCharacter && isCurrentCharacterAlphaNumeric)
    {
        advance(scanner);

        currentCharacter = peek(scanner->current, scanner->sourceLength, scanner->source);
        isCurrentCharacterAlphaNumeric = isAlphaNumeric(currentCharacter);
        isCurrentCharacterTheEndCharacter = isAtEnd(scanner->current, scanner->sourceLength);
    }

    char *literal = getSubstring(scanner->source, scanner->start, scanner->current);
    enum TokenType tokenType = getIdentifierType(literal);

    addToken(scanner, tokenType, literal);

    free(literal); // prevent memory leak
}

char peek(int current, int lengthOfSource, const char *sourceCode)
{
    if (isAtEnd(current, lengthOfSource))
    {
        return '\0'; // Return null char if at end
    }

    return sourceCode[current]; // Look at current character (where current character is not consumed yet)
}

char peekNext(int current, int lengthOfSource, const char *sourceCode)
{
    if (current + 1 >= lengthOfSource)
    {
        return '\0'; // return null character if at end
    }

    return sourceCode[current + 1]; // return the next character
}

char advance(struct Scanner *scanner)
{
    char currentChar = scanner->source[scanner->current]; // Get current character

    if (scanner->current < scanner->sourceLength)
    {
        scanner->current++; // Move to the next character
    }

    // returns the current character but moves the pointer of current to the next character
    return currentChar;
}

bool isAtEnd(int current, int lengthOfSource)
{
    // sourceCode[lengthOfSource] contains the null character '\0'
    return current >= lengthOfSource;
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

enum TokenType getIdentifierType(const char *text)
{
    size_t keywordsCount = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < keywordsCount; i++)
    {
        if (strcmp(text, keywords[i].name) == 0)
        {
            return keywords[i].type;
        }
    }

    return ATOM_IDENTIFIER;
}

char *getSubstring(const char *source, int start, int end)
{
    if (start >= end)
        return NULL; // empty substring

    int length = end - start;
    char *sub = (char *)malloc(length + 1); // +1 for null terminator
    if (!sub)
    {
        return NULL; // check malloc success
    }

    strncpy(sub, source + start, length);
    sub[length] = '\0'; // null terminate

    return sub;
}

void printTokens(struct Scanner scanner)
{
    for (int i = 0; i < scanner.tokenCount; i++)
    {
        struct Token t = scanner.tokens[i];

        if (t.type == ATOM_NUMBER)
        {
            printf("Token %d: type=%d, lexeme=%s, literal=%f, line=%d\n",
                   i, t.type, t.lexeme, t.literal.number, t.line);
        }
        else if (t.type == ATOM_STRING)
        {
            printf("Token %d: type=%d, lexeme=%s, literal=%s, line=%d\n",
                   i, t.type, t.lexeme, t.literal.string, t.line);
        }
        else
        {
            printf("Token %d: type=%d, lexeme=\"%s\", line=%d\n",
                   i, t.type, t.lexeme, t.line);
        }
    }
}

// ================================= End: Function Implementation =================================

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main [script].txt\n");

        /* 64: “command line usage error” – the user gave incorrect arguments */
        return 64;
    }

    runFile(argv[1]);

    return 0;
}