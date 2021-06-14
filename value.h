#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
using namespace std;

enum ValType { VINT, VSTR, VERR };

class Value {
	ValType	T;
	int		I;
	string	S;
    
public:
    Value() : T(VERR), I(0) {}
    Value(int vi) : T(VINT), I(vi) {}
    Value(string vs) : T(VSTR), I(0), S(vs) {}
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsStr() const { return T == VSTR; }
    
    int GetInt() const { if( IsInt() ) return I; throw "RUNTIME ERROR: Value not an integer"; }
    string GetStr() const { if( IsStr() ) return S; throw "RUNTIME ERROR: Value not a string"; }
        
    // add op to this
    Value operator+(const Value& op) const
    {
        Value error;
        //Value output;
        
        if(IsErr())
        {return error;}
        
        if(op.IsErr())
        {return error;}
        
        if(IsInt() && op.IsInt() )
        {
        	Value output(GetInt() + op.GetInt());
            return output;
           
        }
        else if ( IsStr() && op.IsStr() )
        {
            string addstr;
            addstr = GetStr() + op.GetStr();
            Value output(addstr);
            return output;
            //return addstr;
        }
         
        return error;
    };
    
    // subtract op from this
    Value operator-(const Value& op) const
    {
     	Value error;
        
        if(IsErr())
        {return error;}
        
        if(op.IsErr())
        {return error;}
        
        if(IsInt()  && op.IsInt() )
        {
        	Value output(GetInt() - op.GetInt());
            return output;
            //return GetInt() - op.GetInt();
        }
        else if ( IsStr() && op.IsStr())
        {
            return GetStr();
        }
     
        
        
        return error;   
    };
    
    // multiply this by op
    Value operator*(const Value& op) const
    {
        Value error;
        //Value output;
        
        if(IsErr())
        {return error;}
        
        if(op.IsErr())
        {return error;}
        
        if(IsInt() && op.IsInt() )
        {
        	Value output(GetInt() * op.GetInt());
            return output;
           
        }
        else if ( IsStr() && op.IsInt() )
        {
            if( GetInt() < 0 )
            {
                return error;
            }
            
            string multistr;
            for (int x = 0; x < op.GetInt(); x++)
            {
                multistr += GetStr();
            }
            
            Value output(multistr);
            return output;
            //return addstr;
        }
        else if ( IsInt() && op.IsStr() )
        {
            if( GetInt() < 0 )
            {
                return error;
            }
            
        	string multistr;
            for (int x = 0; x < GetInt(); x++)
            {
                multistr += op.GetStr();
            }
            
            Value output(multistr);
            return output;    
        }
         
        return error;
    };
    
    // divide this by op
    Value operator/(const Value& op) const
    {
    	 Value error;
        //Value output;
        
        if(IsErr())
        {return error;}
        
        if(op.IsErr())
        {return error;}
        
        if(IsInt() && op.IsInt() )
        {
        	Value output(GetInt() / op.GetInt());
            return output;
           
        }   
        return error;
    };
    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.I;
        else if( op.IsStr() ) out << op.S;
        else out << "ERROR";
        return out;
    }
};

#endif