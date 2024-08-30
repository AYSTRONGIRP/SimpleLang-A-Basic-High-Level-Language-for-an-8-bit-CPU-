#include "parser.h"
#include "lexer.h"

#include <iostream>
#include<algorithm>
using namespace std;

map<string,int> VariableNode::mem_loc;
int VariableNode::counter = 1;

NumberNode::NumberNode(int val):val(val){};

void NumberNode::generateL(ostream& out){
    out<<"ldi A "<<val<<endl;
    cout<<"ldi A "<<val<<endl;
}

void NumberNode::generateR(ostream& out){
    out<<"ldi B "<<val<<endl;
    cout<<"ldi B "<<val<<endl;
}

VariableNode::VariableNode(string& name):name(name){
    // if(mem_loc.find(name)== mem_loc.end())
    //     mem_loc[name]=counter++;

    id = mem_loc[name];
}

void VariableNode::generateL(ostream& out){
    out<<"mov B M "<<mem_loc[name]<<endl;
}

void VariableNode::generateR(ostream& out){
    out<<"mov b M"<< mem_loc[name]<<endl;
}

void BinaryOpNode::generateCode(ostream& out){
    l->generateL(out);
    r->generateR(out);
    switch(op){
        case '+' : out<<"add"<<endl;break;
        case '-' : out<<"sub"<<endl;break;
        case '#' : out<<"cmp"<<endl;break;
        default:throw runtime_error("unsupported binary op");
    }
}

VariableDec::VariableDec(string varN ,shared_ptr<ASTNode> val):varN(varN),val(val){
    VariableNode::mem_loc[varN]=VariableNode::counter++;
}

void VariableDec::generateCode(ostream& out){

}

ConditionalNode::ConditionalNode(shared_ptr<ASTNode> cond , shared_ptr<ASTNode>then_br , shared_ptr<ASTNode> else_br)
: cond(cond), then_br(then_br), else_br(else_br) {}

void ConditionalNode::generateCode(ostream& out){
    cond->generateCode(out);
    out<<"jnz %ELSEBR"<<endl;
    then_br->generateCode(out);
    out<<"jmp %OUTOFIF"<<endl;
    out<<"ELSEBR:"<<endl;
    if(else_br){
        else_br->generateCode(out);
    }
    out<<"OUTOFIF:"<<endl;
}

void BlockNode::addStat(shared_ptr<ASTNode> statement){
    statements.push_back(statement);
}

void BlockNode::generateCode(ostream& out){
    for(shared_ptr<ASTNode> each : statements){
        each->generateCode(out);
    }
}

Parser::Parser(vector<Token>& token):tokens(tokens),pos(0){}

shared_ptr<ASTNode> Parser::parse(){
    shared_ptr<BlockNode> bNode = shared_ptr<BlockNode>();
    while(!isAtEnd()){
        bNode->addStat(parseStat());
    }
    return bNode;
}

shared_ptr<ASTNode> Parser::parseStat(){
    if(match({Tokentype::INT})){
        return parseVarDec();
    }else if(match({Tokentype::IF})){
        return parseCond();
    }else if(match({Tokentype::IDENTIFIER})){
        return parseVarAssign();
    }else{
        return parseExpStat();
    }
}

shared_ptr<ASTNode> Parser::parseVarDec(){
    consume(Tokentype::IDENTIFIER);
    string varN = previous().value;

    shared_ptr<ASTNode> val = nullptr;

    if(match({Tokentype::EQUAL})){
        val=parseExp();
    }

    consume(Tokentype::SEMICOLON);
    return make_shared<VariableDec>(varN,val);
}

shared_ptr<ASTNode> Parser::parseCond(){
    consume({Tokentype::LPAREN});
    shared_ptr<ASTNode> cond = parseExp();
    consume({Tokentype::RPAREN});
    shared_ptr<ASTNode> then_br = parseBlock();
    shared_ptr<ASTNode> else_br = nullptr;

    if(match({Tokentype::ELSE})){
        else_br=parseBlock();
    }

    make_shared<ConditionalNode>(cond,then_br,else_br);
}

shared_ptr<ASTNode> Parser::parseBlock(){
    if(match({Tokentype::LBRACE})){
        shared_ptr<BlockNode> b = make_shared<BlockNode>();
        
        while(!check(Tokentype::RBRACE) && !isAtEnd()){
            b->addStat(parseStat());
        }   
        consume(Tokentype::RBRACE);
        return b;
    }else{
        return parseStat();
    }
}

shared_ptr<ASTNode> Parser::parseExpStat(){
    shared_ptr<ASTNode> exp = parseExp();
    consume(Tokentype::SEMICOLON);
    return exp;
}

shared_ptr<ASTNode> Parser::parseExp(){
    shared_ptr<ASTNode> l = parsePrim();

    // while(match({Tokentype::EQ})){
    //     char op = previous().value[0];
    //     shared_ptr<ASTNode> r = parsePrim();
    //     l = make_shared<BinaryOpNode>(l,op,r);
    // }

    while(match({Tokentype::PLUS, Tokentype::MINUS ,Tokentype::EQ})){
        char op = previous().value[0];
        shared_ptr<ASTNode> r = parsePrim();
        l = make_shared<BinaryOpNode>(l,op,r);
    }

    return l ;
}

shared_ptr<ASTNode> Parser::parsePrim(){
    if(match({Tokentype::NUMBER})){
        return make_shared<NumberNode>(stoi(previous().value));
    }else if(match({Tokentype::IDENTIFIER})){
        return make_shared<VariableNode>(previous().value);
    }else if(match({Tokentype::LPAREN})){
        shared_ptr<ASTNode> exp = parseExp();
        consume({Tokentype::RPAREN});
        return exp;
    }
    throw runtime_error("unexpected token ");
}

bool Parser::match(vector<Tokentype>& types){
    auto it = find_if(types.begin(),types.end(),[&](Tokentype type){ return type = tokens[pos].type;});
    if(it!=types.end()){
        pos++; return true ;
    }
    return false;
}
bool Parser::isAtEnd(){
    return pos>= tokens.size() || tokens[pos].type == Tokentype::END;
}

bool Parser::check(Tokentype type){
    if(isAtEnd())return false;
    return tokens[pos].type == type ;
}

Token Parser::previous(){
    return tokens[pos-1];
}

void Parser::advance(){
    if(isAtEnd())pos++;
}
void Parser::consume(Tokentype type){
    if(check(type))
        advance();
    else{
        throw runtime_error("wrong token present");
    }
}