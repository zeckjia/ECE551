#include "expr.hpp"

#define NUMTOL 0.00000000001  // numerical tolerance

// help functions for parse
void skipSpace(char ** str);
void skiprSpace(char ** str);
int countParen(char ** str);

// funSet class declaration
class funSet {
  std::vector<Expr *> functions;

  // helper methods for define
  std::vector<std::string> getDefIds(char ** str);
  std::vector<paramExpr> parseDefLhs(char * lhs, std::string & fname);
  Expr * parseDefRhs(char ** str, std::vector<paramExpr> params);
  Expr * makeDefRhsFun(Expr * f_rhs, std::vector<Expr *> exprs);
  Expr * parseDefRhsFun(char ** str, std::vector<paramExpr> params);
  void printDef(Expr * f);

  // helper methods for test
  Expr * parseTest(char ** str);
  Expr * makeTestFun(Expr * f_rhs, std::vector<Expr *> exprs);
  Expr * parseTestFun(char ** str);
  void printTest(Expr * argu1, double argu2);

  // helper methods for integration and gradient ascent and descent
  std::string getFname(char ** str);
  std::vector<double> getNums(char ** str);
  double calFunValue(Expr * f, std::vector<double> pt);

  // helper methods for integration
  std::vector<std::pair<double, double> > getIntRanges(std::vector<double> nums);
  double calInt(Expr * f, std::vector<std::vector<double> > points);
  // numerical integration
  void getNumIntPts(std::vector<std::pair<double, double> > Ranges,
                    double width,
                    std::vector<double> pt,
                    std::vector<std::vector<double> > & points);
  double calNumInt(Expr * f, std::vector<std::vector<double> > points, double width);
  // Monte Carlo integration
  std::vector<std::vector<double> > getMcIntPts(
      std::vector<std::pair<double, double> > Ranges,
      int numPts);
  double calMcInt(Expr * f,
                  std::vector<std::vector<double> > randPts,
                  std::vector<std::pair<double, double> > Ranges);

  // helper methods for gradient ascent and descent
  int getMaxNumStep(char ** str);
  double calDis(std::vector<double> pt1, std::vector<double> pt2);
  std::vector<double> gradient(Expr * f, std::vector<double> pt);
  std::vector<double> gradAstDst(Expr * f,
                                 std::vector<double> pcurr,
                                 double gamma,
                                 double convgDis,
                                 int maxNumStep,
                                 char op);
  void printGrad(std::vector<double> pt);

  // other helper methods
  Expr * lookupFunction(std::string fname);
  void addFunction(Expr * f);

 public:
  funSet();
  funSet(const funSet & rhs);
  funSet & operator=(const funSet & rhs);
  ~funSet();

  // methods to operate command
  void define(char ** str);
  void test(char ** str);
  void numint(char ** str);
  void mcint(char ** str);
  void MaxMin(char ** str, char op);
};
