#include <cstdio>
#include <sstream>
#include <string>

class Expression {
 public:
  virtual ~Expression() {}
  virtual std::string toString() const = 0;
  virtual long evaluate() const = 0;
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
  virtual long evaluate() const { return num; };
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
  virtual long evaluate() const = 0;
};

class PlusExpression : public opExpression {
 public:
  PlusExpression(Expression * l, Expression * r) : opExpression('+', l, r) {}
  virtual ~PlusExpression() {}
  virtual long evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
};

class MinusExpression : public opExpression {
 public:
  MinusExpression(Expression * l, Expression * r) : opExpression('-', l, r) {}
  virtual ~MinusExpression() {}
  virtual long evaluate() const { return lhs->evaluate() - rhs->evaluate(); }
};

class TimesExpression : public opExpression {
 public:
  TimesExpression(Expression * l, Expression * r) : opExpression('*', l, r) {}
  virtual ~TimesExpression() {}
  virtual long evaluate() const { return lhs->evaluate() * rhs->evaluate(); }
};

class DivExpression : public opExpression {
 public:
  DivExpression(Expression * l, Expression * r) : opExpression('/', l, r) {}
  virtual ~DivExpression() {}
  virtual long evaluate() const { return lhs->evaluate() / rhs->evaluate(); }
};
