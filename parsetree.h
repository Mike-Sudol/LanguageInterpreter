/*
 * parsetree.h
 */

#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include "parse.h"
#include "lex.h"
#include "value.h"

class ParseTree{
protected:
	int	linenum;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }

    	virtual bool isPlus() const { return false; }
    	virtual bool isEq() const { return false; }
	virtual bool isIdent() const { return false; }
	virtual bool isVar() const { return false; }
	virtual string getId() const { return ""; }
    
    virtual Value Eval(map<string,Value>& syms)=0;
    
    int MaxDepth() const {
        
        int rightside = 0;
        int leftside = 0;
        if(left)
        {
            leftside += left->MaxDepth();
        }
        if (right)
        {
            rightside += right->MaxDepth();
        }
        
        if (leftside >= rightside)
        {
            return leftside + 1;
        }
       	else 
        {
            return rightside + 1;
        }
    }
    
    int plusCount()
    {
        int counter = 0;
        
        if(left)
        {
            counter += left->plusCount();
        }
        
        if(right)
        {
            counter += right->plusCount();
        }
        
        if(isPlus())
        {
            counter++;
        }
        return counter;
    }
    
    int equalsCount()
    {
         int counter = 0;
        
        if(left)
        {
            counter += left->equalsCount();
        }
        
        if(right)
        {
            counter += right->equalsCount();
        }
        
        if(isEq())
        {
            counter++;
        }
        return counter;
    }

};

class StmtList : public ParseTree {
public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    Value Eval(map<string,Value>& syms)
    {
        left->Eval(syms);
        
        if(right)
        {
            right->Eval(syms);
        }
        return Value();
    }
};

class Print : public ParseTree{
public:
	Print(int line, ParseTree *e) : ParseTree(line, e) {}
    Value Eval(map<string,Value>& syms)
    {
        cout << left->Eval(syms);
        return Value();
    }
};

class Println : public ParseTree { 
public:
    Println(int line, ParseTree *e) : ParseTree(line, e) {}
    Value Eval(map<string,Value>& syms)
    {
        cout << left->Eval(syms) << endl;
        return Value();
    }
};

class Repeat : public ParseTree {
public: 
    Repeat(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    Value Eval(map<string,Value>& syms)
    {
        Value test;
        return test;        
    }
};

class Assign : public ParseTree {
public:
    Assign(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
	bool isEq() const {return true;}
    Value Eval(map<string,Value>& syms)
    {
        Value lefty = left->Eval(syms);
        Value righty = right->Eval(syms);
        
        syms[lefty.GetStr()] = righty;
        return Value();
    }
};

class PlusExpr : public ParseTree {
public:
    PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
	bool isPlus() const {return true;}
    Value Eval(map<string,Value>& syms)
    {
        return left->Eval(syms) + right->Eval(syms);        
    }
};

class MinusExpr : public ParseTree { 
public:
    MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    Value Eval(map<string,Value>& syms)
    {
        return left->Eval(syms) - right->Eval(syms);        
    }    

};

class TimesExpr : public ParseTree { 
public:
    TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval(map<string,Value>& syms)
    {
        return left->Eval(syms) * right->Eval(syms);        
    }
};

class DivideExpr : public ParseTree { 
public: 
    DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
    Value Eval(map<string,Value>& syms)
    {
        return left->Eval(syms) / right->Eval(syms); 
    }
};

class IConst : public ParseTree {
	int val;

public:
	IConst(Tok& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    Value Eval(map<string,Value>& syms)
    {
        return Value(val);
    }
};

class SConst : public ParseTree {
	string val;

public:
	SConst(Tok& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    Value Eval(map<string,Value>& syms)
    {
       return Value(val);        
    }
};

class Ident : public ParseTree {
    string id;
    
public: 
    Ident(Tok& t) : ParseTree(t.GetLinenum()){
    	id = t.GetLexeme();
    }
    Value Eval(map<string,Value>& syms)
    {
        if(syms.find(id) != syms.end() )
        {
            return syms[id];
        }
        
        return Value(id);
    }
};

#endif /* PT_H_ */