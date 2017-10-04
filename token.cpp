#include "token.h"
TOKEN getToken ()
{
        int type = yylex();
        YYSTYPE val = yylval;
        TOKEN newToken;
        newToken.type = type;
        newToken.lval = val;
        return newToken;
}
void printToken(TOKEN token)
{
    int token_type = token.type;
    if(token_type == ID)
        printf("ID<%s> ", token.lval.c_str());
    else if (token_type == NUM)
        printf("NUM<%s> ", token.lval.c_str());
    else if (token_type == ERROR)
        printf("ERROR at column %s", token.lval.c_str());
    else
        printf("%s ", token_strs[token_type - 257]);
}
struct yy_buffer_state {
    FILE * 	yy_input_file;
    char * 	yy_ch_buf;
    char * 	yy_buf_pos;
    size_t 	yy_buf_size;
    int 	yy_n_chars;
    int 	yy_is_our_buffer;
    int 	yy_is_interactive;
    int 	yy_at_bol;
    int 	yy_fill_buffer;
    int 	yy_buffer_status;
};

typedef yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_buffer(char *base, size_t size);

void scan(std::string str) {
    char* buffer = (char *) malloc(str.length() + 2);
    std::memset(buffer, 0, str.length() + 2);
    std::strcpy(buffer, str.c_str());
    yy_scan_buffer(buffer, str.length() + 2);
}
