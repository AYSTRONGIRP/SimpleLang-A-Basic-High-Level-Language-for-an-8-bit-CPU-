#include <ostream>
#include <map>
#include <vector>
#include "lexer.h"
#ifndef PARSER_H
#define PARSER_H
using namespace std;
class ASTNode {
    public:
        virtual void generateCode(ostream& out) = 0 ;
        virtual void generateL(ostream& out) = 0 ;
        virtual void generateR(ostream& out) = 0 ;
};

class NumberNode : public ASTNode{
    public:
        int val;
        NumberNode(int val);
};

class VariableNode : public ASTNode {
    public:
        static map<string,int> mem_loc;
        static int counter;

        string name;
        int id ;
        VariableNode(string& name);
};

class VariableDec : public VariableNode{
    public:
        string varN ;
        shared_ptr<ASTNode> val ;
        VariableDec(string varN , shared_ptr<ASTNode> val);
};

class BinaryOpNode : public ASTNode {
    public:
        shared_ptr<ASTNode> l , r ;
        char op ;
        BinaryOpNode(shared_ptr<ASTNode> l, char op , shared_ptr<ASTNode> r);
};

class ConditionalNode : public ASTNode {
    public:
        shared_ptr<ASTNode> cond , then_br , else_br ;
        ConditionalNode(shared_ptr<ASTNode>cond , shared_ptr<ASTNode> then_br , shared_ptr<ASTNode> else_br);
};

class Parser {
    public:
        Parser(vector<Token>& tokens);
        shared_ptr<ASTNode> parse();
    
    private:
        vector<Token> tokens;
        size_t pos ;

        shared_ptr<ASTNode> parseExp();
        shared_ptr<ASTNode> parsePrim();
        shared_ptr<ASTNode> parseStat();
        shared_ptr<ASTNode> parseVarDec();
        shared_ptr<ASTNode> parseVarAssign();
        shared_ptr<ASTNode> parseCond();
        
        bool match(vector<Tokentype>& types);
        bool isAtEnd();
        bool check(Tokentype type);
        Token advance();
        Token peek();
        Token previous();
        void consume(Tokentype type);
};


#endif