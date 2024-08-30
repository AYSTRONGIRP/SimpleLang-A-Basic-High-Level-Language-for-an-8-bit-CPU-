#include "lexer.h"
#include <iostream>
using namespace std ;

Lexer::Lexer(string& source) : source(source), pos(0) {}

vector<Token> Lexer::tokenize(){
    vector<Token> tokens;
    while(pos < source.length()){
            char cur = source[pos];
            if(isspace(cur)){
                pos++;
                continue;
            }

            if(isalpha(cur)){
                string identifier ;
                while(isalnum(cur)){
                    identifier += cur;
                    pos++;
                    if(pos>=source.length())
                        break;
                    cur = source[pos];
                }
                if(identifier == "int") tokens.push_back({Tokentype::INT,"int"});
                else if (identifier =="if") tokens.push_back({Tokentype::IF,"if"});
                else if (identifier == "else") tokens.push_back({Tokentype::ELSE,"else"});
                else tokens.push_back({Tokentype::IDENTIFIER,identifier});
            } else if (isdigit(cur)){
                string num;
                while(isdigit(cur)){
                    num+=cur;
                    pos++;
                    if(pos>=source.length())
                        break;
                    
                    cur = source[pos];
                }
                tokens.push_back({Tokentype::NUMBER,num});
            } else{
                    switch(cur){
                        case '=':
                        pos++;
                        if(pos<source.length() && source[pos] == '='){
                            tokens.push_back({Tokentype::EQ,"#"}); //== will not work for my binary node as switch need char const
                            // pos++;
                        }else{
                            tokens.push_back({Tokentype::EQUAL,"="});
                            cout<<"in equal too"<<endl;
                            pos--;
                        }
                        break;
                        case '+':tokens.push_back({Tokentype::PLUS,"+"});break;
                        case '-':tokens.push_back({Tokentype::MINUS,"-"});break;
                        case '(':tokens.push_back({Tokentype::LPAREN,"("});break;
                        case ')':tokens.push_back({Tokentype::RPAREN,")"});break;
                        case '{':tokens.push_back({Tokentype::LBRACE,"{"});break;
                        case '}':tokens.push_back({Tokentype::RBRACE,"}"});break;
                        case ';':tokens.push_back({Tokentype::SEMICOLON,";"});break;
                        default: cerr<< "unknown character" <<cur <<endl;
                    }
                    pos++;
                }
    }
    tokens.push_back({Tokentype::END,""});
    return tokens;
}