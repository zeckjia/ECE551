#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// template function used for swapping two Ts
template<typename T>
void swap(T & lhs, T & rhs) {
  T temp = lhs;
  lhs = rhs;
  rhs = temp;
}

class paramExpr;

// expression class (abstract)
class Expr {
 public:
  virtual std::string & getName() = 0;
  virtual Expr *& getExpr() = 0;
  virtual paramExpr & lookupParam(std::string _pName) = 0;
  virtual std::vector<paramExpr> & getParams() = 0;
  virtual void passInValues(Expr * outFun) = 0;

  virtual int
  exprType() = 0;  // expression type, numExpr is 0, paramExpr is 1, preDefFunExpr is 2,  userDefFunExpr is 3
  virtual Expr * clone() = 0;          // used for deep copy
  virtual std::string toString() = 0;  // change expression to output string
  virtual ~Expr() {}
  virtual double evaluate() = 0;  // used value calculation
};

// number expression class (concrete)
class numExpr : public Expr {
 private:
  double num;

 public:
  // constructor
  numExpr(double _num);

  // virtual methods
  virtual std::string & getName() {
    std::cerr << "A 'numExpr' cannot not call 'getName'.\n";
    throw std::exception();
  }
  virtual Expr *& getExpr() {
    std::cerr << "A 'numExpr' cannot not call 'getExpr'.\n";
    throw std::exception();
  }
  virtual paramExpr & lookupParam(std::string _pName) {
    std::cerr << "A 'numExpr' cannot not call 'loopupParam'.\n";
    throw std::exception();
  }
  virtual std::vector<paramExpr> & getParams() {
    std::cerr << "A 'numExpr' cannot not call 'getParams'.\n";
    throw std::exception();
  }
  virtual void passInValues(Expr * outFun) {
    std::cerr << "A 'numExpr' cannot not call 'passInValues'.\n";
    throw std::exception();
  }

  virtual int exprType() { return 0; }
  virtual Expr * clone() { return new numExpr(num); }
  virtual std::string toString() {
    std::stringstream result;
    result << num;
    return result.str();
  }
  virtual ~numExpr() {}
  virtual double evaluate() { return num; }
};

// parameter expression class (concrete)
class paramExpr : public Expr {
 private:
  std::string pName;
  Expr * expr;  // inner expression

 public:
  // constructors and operator=
  paramExpr(std::string _pName);
  paramExpr(const paramExpr & rhs);
  paramExpr & operator=(const paramExpr & rhs);

  // virtual methods
  virtual std::string & getName() { return pName; }
  virtual Expr *& getExpr() { return expr; }
  virtual paramExpr & lookupParam(std::string _pName) {
    std::cerr << "A 'paramExpr' cannot not call 'loopupParam'.\n";
    throw std::exception();
  }
  virtual std::vector<paramExpr> & getParams() {
    std::cerr << "A 'paraExpr' cannot not call 'getParams'.\n";
    throw std::exception();
  }
  virtual void passInValues(Expr * outFun) {
    std::cerr << "A 'paramExpr' cannot not call 'passInValues'.\n";
    throw std::exception();
  }

  virtual int exprType() { return 1; }  // expression type, numExpr is 1
  virtual Expr * clone() { return new paramExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << expr->toString();
    return result.str();
  }
  virtual ~paramExpr() { delete expr; }
  virtual double evaluate() { return expr->evaluate(); }
};

// function expression class (abstract)
class funExpr : public Expr {
 protected:
  std::string fname;
  std::vector<paramExpr> params;  // parameters

 public:
  // constructors
  funExpr(std::string _fname, std::vector<paramExpr> _params);
  funExpr(const funExpr & rhs);

  // virtual methods
  virtual std::string & getName() { return fname; }
  virtual Expr *& getExpr() = 0;
  virtual paramExpr & lookupParam(std::string _pName) {
    for (size_t i = 0; i < params.size(); i++) {
      if (_pName == params[i].getName()) {
        return params[i];
      }
    }
    std::cerr << "Could not find the parameter!\n";
    throw std::exception();
  }
  virtual std::vector<paramExpr> & getParams() { return params; }
  virtual void passInValues(Expr * outFun) {  // pass assigned values to inner expressions
    for (size_t i = 0; i < getParams().size(); i++) {
      int typeNum = getParams()[i].getExpr()->exprType();
      switch (typeNum) {
        case 0: {
          break;
        }
        case 1: {
          // assign expressions to parameters
          paramExpr & _param = outFun->lookupParam(getParams()[i].getExpr()->getName());
          if (getParams()[i].getExpr()->getExpr() != NULL) {
            delete getParams()[i].getExpr()->getExpr();
          }
          getParams()[i].getExpr()->getExpr() = _param.clone();
          break;
        }
        case 2: {
          getParams()[i].getExpr()->passInValues(outFun);
          break;
        }
        case 3: {
          getParams()[i].getExpr()->passInValues(outFun);
          break;
        }
      }
    }
  }

  virtual int exprType() = 0;
  virtual Expr * clone() = 0;
  virtual std::string toString() = 0;
  virtual ~funExpr() {}
  virtual double evaluate() = 0;
};

// user-defined function expression class (concrete)
class userDefFunExpr : public funExpr {
 private:
  Expr * fExpr;

 public:
  // constructors
  userDefFunExpr(std::string _fname, std::vector<paramExpr> _params);
  userDefFunExpr(const userDefFunExpr & rhs);
  userDefFunExpr & operator=(const userDefFunExpr & rhs);

  // virtual methods
  virtual Expr *& getExpr() { return fExpr; }
  virtual void passInValues(Expr * outFun) {
    funExpr::passInValues(outFun);
    fExpr->passInValues(this);
  }

  virtual int exprType() { return 3; }  // expression type, numExpr is 3
  virtual Expr * clone() { return new userDefFunExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << fname << "(";
    for (size_t i = 0; i < params.size(); i++) {
      result << params[i].toString();
      if (i != params.size() - 1) {
        result << ' ';
      }
    }
    result << ")";
    return result.str();
  }
  virtual ~userDefFunExpr() { delete fExpr; }
  virtual double evaluate() { return fExpr->evaluate(); }
};

// pre-defined function expression class (abstract)
class preDefFunExpr : public funExpr {
 public:
  // constructor
  preDefFunExpr(std::string _fname, std::vector<paramExpr> _params);

  // virtual method
  virtual Expr *& getExpr() {
    std::cerr << "A 'preDefFunExpr' cannot not call 'getExpr'.\n";
    throw std::exception();
  }

  virtual int exprType() { return 2; }  // expression type, numExpr is 2
  virtual Expr * clone() = 0;
  virtual std::string toString() = 0;
  virtual ~preDefFunExpr() {}
  virtual double evaluate() = 0;
};

// plus class (concrete)
class plusExpr : public preDefFunExpr {
 public:
  plusExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new plusExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "(" << params[0].toString() << " + " << params[1].toString() << ")";
    return result.str();
  }
  virtual ~plusExpr() {}
  virtual double evaluate() { return params[0].evaluate() + params[1].evaluate(); }
};

// minus class (concrete)
class minusExpr : public preDefFunExpr {
 public:
  minusExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new minusExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "(" << params[0].toString() << " - " << params[1].toString() << ")";
    return result.str();
  }
  virtual ~minusExpr() {}
  virtual double evaluate() { return params[0].evaluate() - params[1].evaluate(); }
};

// times class (concrete)
class timesExpr : public preDefFunExpr {
 public:
  timesExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new timesExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "(" << params[0].toString() << " * " << params[1].toString() << ")";
    return result.str();
  }
  virtual ~timesExpr() {}
  virtual double evaluate() { return params[0].evaluate() * params[1].evaluate(); }
};

// division class (concrete)
class divExpr : public preDefFunExpr {
 public:
  divExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new divExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "(" << params[0].toString() << " / " << params[1].toString() << ")";
    return result.str();
  }
  virtual ~divExpr() {}
  virtual double evaluate() { return params[0].evaluate() / params[1].evaluate(); }
};

// modulus class (concrete)
class modExpr : public preDefFunExpr {
 public:
  modExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new modExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "(" << params[0].toString() << " % " << params[1].toString() << ")";
    return result.str();
  }
  virtual ~modExpr() {}
  virtual double evaluate() {
    // no mod operation for doubles, change to ints
    return (int)params[0].evaluate() % (int)params[1].evaluate();
  }
};

// power class (concrete)
class powExpr : public preDefFunExpr {
 public:
  powExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new powExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "(" << params[0].toString() << " ^ " << params[1].toString() << ")";
    return result.str();
  }
  virtual ~powExpr() {}
  virtual double evaluate() {
    return std::pow(params[0].evaluate(), params[1].evaluate());
  }
};

// floor class (concrete)
class floorExpr : public preDefFunExpr {
 public:
  floorExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new floorExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "floor"
           << "(" << params[0].toString() << ")";
    return result.str();
  }
  virtual ~floorExpr() {}
  virtual double evaluate() { return std::floor(params[0].evaluate()); }
};

// ceil class (concrete)
class ceilExpr : public preDefFunExpr {
 public:
  ceilExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new ceilExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "ceil"
           << "(" << params[0].toString() << ")";
    return result.str();
  }
  virtual ~ceilExpr() {}
  virtual double evaluate() { return std::ceil(params[0].evaluate()); }
};

// round class (concrete)
class roundExpr : public preDefFunExpr {
 public:
  roundExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new roundExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "round"
           << "(" << params[0].toString() << ")";
    return result.str();
  }
  virtual ~roundExpr() {}
  virtual double evaluate() { return round(params[0].evaluate()); }
};

// logb class (concrete)
class logbExpr : public preDefFunExpr {
 public:
  logbExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new logbExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "("
           << "logb " << params[0].toString() << " " << params[1].toString() << ")";
    return result.str();
  }
  virtual ~logbExpr() {}
  virtual double evaluate() {
    return std::log(params[0].evaluate()) / std::log(params[1].evaluate());
  }
};

// select class (concrete)
class selectExpr : public preDefFunExpr {
 public:
  selectExpr(std::vector<paramExpr> _params);

  virtual Expr * clone() { return new selectExpr(*this); }
  virtual std::string toString() {
    std::stringstream result;
    result << "("
           << "select " << params[0].toString() << " " << params[1].toString() << " "
           << params[2].toString() << ")";
    return result.str();
  }
  virtual ~selectExpr() {}
  virtual double evaluate() {
    if (params[0].evaluate() >= 0) {
      return params[1].evaluate();
    }
    else {
      return params[2].evaluate();
    }
  }
};
