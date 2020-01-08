#include "expr.hpp"

// numExpr class
numExpr::numExpr(double _num) : num(_num) {
}

// paramExpr class
paramExpr::paramExpr(std::string _pName) : pName(_pName), expr(NULL) {
}

paramExpr::paramExpr(const paramExpr & rhs) : pName(rhs.pName), expr(NULL) {
  if (rhs.expr != NULL) {
    expr = rhs.expr->clone();
  }
}

paramExpr & paramExpr::operator=(const paramExpr & rhs) {
  if (this != &rhs) {
    paramExpr temp(rhs);
    swap(pName, temp.pName);
    swap(expr, temp.expr);
  }
  return *this;
}

// function expression class
funExpr::funExpr(std::string _fname, std::vector<paramExpr> _params) :
    fname(_fname),
    params(_params) {
}

funExpr::funExpr(const funExpr & rhs) : fname(rhs.fname), params(rhs.params) {
}

// user-defined function expression class
userDefFunExpr::userDefFunExpr(std::string _fname, std::vector<paramExpr> _params) :
    funExpr(_fname, _params),
    fExpr(NULL) {
}

userDefFunExpr::userDefFunExpr(const userDefFunExpr & rhs) :
    funExpr(rhs.fname, rhs.params),
    fExpr(rhs.fExpr->clone()) {
}

userDefFunExpr & userDefFunExpr::operator=(const userDefFunExpr & rhs) {
  if (this != &rhs) {
    userDefFunExpr temp(rhs);
    swap(fname, temp.fname);
    swap(params, temp.params);
    swap(fExpr, temp.fExpr);
  }
  return *this;
}

// pre-defined function expression class
preDefFunExpr::preDefFunExpr(std::string _fname, std::vector<paramExpr> _params) :
    funExpr(_fname, _params) {
}

// plus class
plusExpr::plusExpr(std::vector<paramExpr> _params) : preDefFunExpr("+", _params) {
  assert(params.size() == 2);
}

// minus class
minusExpr::minusExpr(std::vector<paramExpr> _params) : preDefFunExpr("-", _params) {
  assert(params.size() == 2);
}

// times class
timesExpr::timesExpr(std::vector<paramExpr> _params) : preDefFunExpr("*", _params) {
  assert(params.size() == 2);
}

// div class
divExpr::divExpr(std::vector<paramExpr> _params) : preDefFunExpr("/", _params) {
  assert(params.size() == 2);
}

// mod class
modExpr::modExpr(std::vector<paramExpr> _params) : preDefFunExpr("%", _params) {
  assert(params.size() == 2);
}

// pow class
powExpr::powExpr(std::vector<paramExpr> _params) : preDefFunExpr("^", _params) {
  assert(params.size() == 2);
}

// floor class
floorExpr::floorExpr(std::vector<paramExpr> _params) : preDefFunExpr("floor", _params) {
  assert(params.size() == 1);
}

// ceil class
ceilExpr::ceilExpr(std::vector<paramExpr> _params) : preDefFunExpr("ceil", _params) {
  assert(params.size() == 1);
}

// round class
roundExpr::roundExpr(std::vector<paramExpr> _params) : preDefFunExpr("round", _params) {
  assert(params.size() == 1);
}

// logb class
logbExpr::logbExpr(std::vector<paramExpr> _params) : preDefFunExpr("logb", _params) {
  assert(params.size() == 2);
}

// select class
selectExpr::selectExpr(std::vector<paramExpr> _params) :
    preDefFunExpr("select", _params) {
  assert(params.size() == 3);
}
