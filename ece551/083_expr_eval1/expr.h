#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

class Expression {
 public:
  virtual ~Expression() {}
  virtual std::string toString() const = 0;
};

class NumExpression : public Expression {
 private:
  long num;

 public:
  NumExpression(long n) : num(n) {}
  virtual ~NumExpression() {}
  virtual std::string toString() const {
    std::stringstream ss;
    ss << num;
    return ss.str();
  }
};

class opExpression : public Expression {
 protected:
  char op;
  Expression * lhs;
  Expression * rhs;

 public:
  opExpression(char o, Expression * l, Expression * r) : op(o), lhs(l), rhs(r) {}
  virtual ~opExpression() {
    delete lhs;
    delete rhs;
  }
  virtual std::string toString() const {
    std::stringstream ss;
    ss << '(' << lhs->toString() << ' ' << op << ' ' << rhs->toString() << ')';
    return ss.str();
  }
};

class PlusExpression : public opExpression {
 public:
  PlusExpression(Expression * l, Expression * r) : opExpression('+', l, r) {}
  virtual ~PlusExpression() {}
};
