#include "String.h"
#include <iostream>

using namespace std;

static String Token_Types[] = {
	"NUMBER",
	"SYMBOL",
	"KEYWORD",
	"STRING",
	"END",
	"INVALID"
};

enum TokenType {
	NUMBER,
	SYMBOL,
	KEYWORD,
	STRING,
	END, // special marker for end of input
	INVALID, // this token type should never appear and represents a parsing error
};

class Token {
public:
	TokenType tokenType;
	int numValue;
	String tokenString;

    Token() {
        this->tokenType = INVALID;
        this->numValue = 0;
        this->tokenString = "\0";
    }

	Token(TokenType token_type, int token_number_value, const String &token_string) {
		this->tokenType = token_type;
		this->numValue = token_number_value;
		this->tokenString = token_string;
	}

	void print() {
		if (tokenType == NUMBER) {
			cout << Token_Types[tokenType].c_str() << " : " << numValue << endl;
		} else {
			cout << Token_Types[tokenType].c_str() << " : " << tokenString.c_str() << endl;
		}
	}
};

/*
 */
extern Token* curToken;

/* the next_token function returns a pointer to a null-terminated string that repesents
 * the actual raw token. When next_token_type is equal to NUMBER, the next_token string
 * is unecessary, since token_number_value is available. Nevertheless, in that case, next_token()
 * returns a string representation of the number. When next_token_type is either SYMBOL
 * or NAME, then you must use next_token to get the actual token's value
 *
 * NOTE: the token "//" is an actual token in Blip with type "SYMBOL". You must parse
 * this token and the comment that it precedes.
 */

/* the skip_line function forces the input to ignore the remainder of the current line of
 * input. In ordinary usage, read_next_token() ignores spaces, tabs and line breaks. However
 * when parsing a // comment, you must ignore any token or tokens that remain on the current
 * line. The skip_line is provided for this task
 */
void skip_line();

/* set_input allows you to open a text file to read tokens from. By default, read_next_token
 * reads tokens from the standard input (e.g., the keyboard). The argument provided to set_input
 * must be the name of a readable file containing a valid Blip program.
 */
void set_input(const char*);

/* read_next_token() is forces the Input.cpp infrastruture to advance to the next input token
 * This function reads characters from the input source until a complete token has been read
 * (and may read additional characters as well), and forms a token from those characters.
 * read_next_token does not return a value. Programmers must use the other functions and
 * the global variables to determine which token was read and whether there was an error
 */
void read_next_token();

/*
 * the peek_next_token() function returns the text of the next token without "reading" the token
 * In other words, the peek function let's you look at the next token that will be read when
 * read_next_token is called, without actually read (and removing) that token from the input
 */
String peek_next_token();
