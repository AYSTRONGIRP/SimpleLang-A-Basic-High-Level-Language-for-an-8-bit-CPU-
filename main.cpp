#include<iostream>
#include<fstream>
#include "lexer.h"
#include "parser.h"

void printTokens(vector<Token>& tokens){
    for(Token t:tokens){
        cout<<"Type "<<t.value<<endl;
    }
}
int main(){
    ifstream inputFile("input.txt");
    ofstream outFile("\\wsl.localhost\\Ubuntu-22.04\root\\8bit-computer\\tests\\output.asm");

    if(!outFile.is_open()){
        cerr<<"output file not open"<<endl;
    }

    string sourceCode((istreambuf_iterator<char>(inputFile)),istreambuf_iterator<char>());
    inputFile.close();
    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();

    printTokens(tokens);

    Parser parser(tokens);
    shared_ptr<ASTNode> ast = parser.parse();

    if(ast){
        ast->generateCode(outFile);
    }else{
        cerr<<"parsing failed"<<endl;
    }

    return 0;
}