#include "function.hpp"

#include <string.h>

#include <exception>
#include <random>

// use this function to skip the spaces ahead of a string
void skipSpace(char ** str) {
  while (**str == ' ') {
    (*str)++;
  }
}

// use this function to skip the spaces at the end of a string
void skiprSpace(char ** str) {
  char * pEnd = *str;
  while (*pEnd != '\0') {
    pEnd++;
  }
  pEnd--;
  while (*pEnd == ' ') {
    *pEnd = '\0';
    pEnd--;
  }
}

// this function check if the parentheses appear in pairs, it returns
// number of left parentheses - number of right parentheses
int countParen(char ** str) {
  int lP = 0;
  int rP = 0;
  for (size_t i = 0; i < strlen(*str); i++) {
    if ((*str)[i] == '(') {
      lP++;
    }
    else if ((*str)[i] == ')') {
      rP++;
    }
  }
  return lP - rP;
}

// definitions of methods in funSet class

// helper methods for define
// get identifiers on left hand side of define
std::vector<std::string> funSet::getDefIds(char ** str) {
  std::vector<std::string> ids;

  char * pBegin = *str;
  char * pEnd = *str + strlen(pBegin) - 1;
  if (*pBegin != '(' || *pEnd != ')') {
    std::cerr
        << "Left hand side of function definition must be enclosed by parentheses!\n";
    throw std::exception();
  }
  pBegin++;
  *pEnd = ' ';
  // check if ids contain valid chars
  char * ptr = pBegin;
  while (ptr < pEnd) {
    if (!isdigit(*ptr) && (!isalpha(*ptr)) && *ptr != ' ') {
      std::cerr << "Ids can only contain letters and numbers!\n";
      throw std::exception();
    }
    ptr++;
  }
  // get ids
  skipSpace(&pBegin);
  while (pBegin < pEnd) {
    if (!isalpha(*pBegin)) {
      std::cerr << "An id must start with a letter!\n";
      throw std::exception();
    }
    ptr = strchr(pBegin, ' ');
    ids.push_back(std::string(pBegin, ptr - pBegin));
    pBegin = ptr;
    skipSpace(&pBegin);
  }

  return ids;
}

// parse left hand side of define, return a vector containing all parameters
std::vector<paramExpr> funSet::parseDefLhs(char * lhs, std::string & fname) {
  skipSpace(&lhs);
  skiprSpace(&lhs);
  if (countParen(&lhs) != 0) {
    std::cerr << "Parentheses should appear in pairs\n";
    throw std::exception();
  }
  std::vector<std::string> ids = getDefIds(&lhs);
  if (ids.size() < 2) {
    std::cerr << "A function must contain at least one parameter!\n";
    throw std::exception();
  }
  fname = ids[0];
  std::vector<paramExpr> params;
  for (size_t i = 1; i < ids.size(); i++) {
    params.push_back(paramExpr(ids[i]));
  }

  return params;
}

// parse right hand side of define, determine to make function call, parameter or numeric constant
Expr * funSet::parseDefRhs(char ** str, std::vector<paramExpr> params) {
  skipSpace(str);
  if (**str == '\0') {
    std::cerr << "Incomplete function definition!\n";
    return NULL;
  }
  // function call
  else if (**str == '(') {
    (*str)++;
    return parseDefRhsFun(str, params);
  }
  // parameter
  else if (isalpha(**str)) {
    char * endp = *str;
    while (isalpha(*endp) || isdigit(*endp) || *endp == '-') {
      endp++;
    }
    if (*endp != ' ' && *endp != ')') {
      std::cerr << "Invalid parameter expression!\n";
      return NULL;
    }
    std::string pName(*str, endp - *str);
    // check if parameter is a member of paramter set on left hand side
    for (size_t i = 0; i < params.size(); i++) {
      if (params[i].getName() == pName) {
        break;
      }
      if (i == params.size() - 1) {
        std::cerr << "Cound not find '" << pName << "' in parameter list!\n";
        return NULL;
      }
    }
    *str = endp;
    return new paramExpr(pName);
  }
  // numeric constant
  else if (isdigit(**str) || **str == '-') {
    char * endp;
    double num = strtod(*str, &endp);
    if (*endp != ' ' && *endp != ')') {
      std::cerr << "Invalid number expression!\n";
      return NULL;
    }
    *str = endp;
    return new numExpr(num);
  }
  else {
    std::cerr << "Invalid function call expression!\n";
    return NULL;
  }
}

// make function expression (right hand side of define)
Expr * funSet::makeDefRhsFun(Expr * f_rhs, std::vector<Expr *> exprs) {
  Expr * f = f_rhs->clone();
  for (size_t i = 0; i < exprs.size(); i++) {
    f->getParams()[i].getExpr() = exprs[i];
  }
  return f;
}

// parse function call (right hand side of define)
Expr * funSet::parseDefRhsFun(char ** str, std::vector<paramExpr> params) {
  skipSpace(str);
  char * endp_ = strchr(*str, ' ');
  char * endpP = strchr(*str, '(');
  char * endp = NULL;
  if (endp_ == NULL && endpP == NULL) {
    std::cerr << "Improper format of function definition (no space or left parenthesis "
                 "found after function name)!\n";
    return NULL;
  }
  else if (endpP == NULL) {
    endp = endp_;
  }
  else if (endp_ == NULL) {
    endp = endpP;
  }
  else {
    endp = std::min(endp_, endpP);
  }
  std::string fname(*str, endp - *str);
  Expr * f_rhs = lookupFunction(fname);
  if (f_rhs == NULL) {
    std::cerr << "Function '" << fname << "' was not defined!\n";
    return NULL;
  }

  *str = endp;
  std::vector<Expr *> exprs(f_rhs->getParams().size(), NULL);
  for (size_t i = 0; i < exprs.size(); i++) {
    exprs[i] = parseDefRhs(str, params);
    if (exprs[i] == NULL) {
      for (size_t j = 0; j <= i; j++) {
        delete exprs[j];
      }
      return NULL;
    }
    skipSpace(str);
  }

  if (**str == ')') {
    (*str)++;
    return makeDefRhsFun(f_rhs, exprs);
  }
  std::cerr << "Can not find ')'!\n";
  for (size_t i = 0; i < exprs.size(); i++) {
    delete exprs[i];
  }
  return NULL;
}

// print format if define succeeds
void funSet::printDef(Expr * f) {
  std::cout << "defined " << f->getName() << '(';
  for (size_t i = 0; i < f->getParams().size(); i++) {
    std::cout << f->getParams()[i].getName();
    if (i != f->getParams().size() - 1) {
      std::cout << ' ';
    }
    else {
      std::cout << ')' << '\n';
    }
  }
}

// helper methods for test
// parse 1st argument of test, determine to make function call or numeric constant
Expr * funSet::parseTest(char ** str) {
  skipSpace(str);
  if (**str == '\0') {
    std::cerr << "Incomplete function call!\n";
    return NULL;
  }
  // function call
  else if (**str == '(') {
    (*str)++;
    return parseTestFun(str);
  }
  // numeric constant
  else if (isdigit(**str) || **str == '-') {
    char * endp;
    double num = strtod(*str, &endp);
    if (*endp != ' ' && *endp != ')') {
      std::cerr << "Invalid number expression!\n";
      return NULL;
    }
    *str = endp;
    return new numExpr(num);
  }
  else {
    std::cerr << "Invalid function call expression!\n";
    return NULL;
  }
}

// make function expression (1st argument of test)
Expr * funSet::makeTestFun(Expr * f_rhs, std::vector<Expr *> exprs) {
  Expr * f = f_rhs->clone();
  for (size_t i = 0; i < exprs.size(); i++) {
    f->getParams()[i].getExpr() = exprs[i];
  }
  if (f->exprType() == 3) {
    f->getExpr()->passInValues(f);
  }
  return f;
}

// parse function call (1st argument of test)
Expr * funSet::parseTestFun(char ** str) {
  skipSpace(str);
  char * endp = strchr(*str, ' ');
  if (endp == NULL) {
    std::cerr << "Improper format of function definition (no space found after function "
                 "name)!\n";
    return NULL;
  }
  std::string fname(*str, endp - *str);
  Expr * f_rhs = lookupFunction(fname);
  if (f_rhs == NULL) {
    std::cerr << "Function '" << fname << "' was not defined!\n";
    return NULL;
  }

  *str = endp;
  std::vector<Expr *> exprs(f_rhs->getParams().size(), NULL);
  for (size_t i = 0; i < exprs.size(); i++) {
    exprs[i] = parseTest(str);
    if (exprs[i] == NULL) {
      for (size_t j = 0; j <= i; j++) {
        delete exprs[j];
      }
      return NULL;
    }
    skipSpace(str);
  }

  if (**str == ')') {
    (*str)++;
    return makeTestFun(f_rhs, exprs);
  }
  std::cerr << "Can not find ')'!\n";
  for (size_t i = 0; i < exprs.size(); i++) {
    delete exprs[i];
  }
  return NULL;
}

// print the result of test
void funSet::printTest(Expr * argu1, double argu2) {
  std::cout << argu1->toString() << " = " << argu1->evaluate();
  if (std::abs(argu1->evaluate() - argu2) <= NUMTOL) {
    std::cout << " [correct]\n";
  }
  else {
    std::cout << " [INCORRECT: expected " << argu2 << "]\n";
  }
}

// helper methods for integral and gradient ascent and descent
// get function name
std::string funSet::getFname(char ** str) {
  skipSpace(str);
  skiprSpace(str);
  char * endp = strchr(*str, ' ');
  if (endp == NULL) {
    std::cerr << "Invalid command!\n";
    throw std::exception();
  }
  std::string fname(*str, endp);
  *str = endp;

  return fname;
}

// get numbers(double)
std::vector<double> funSet::getNums(char ** str) {
  std::vector<double> nums;

  char * endp;
  skipSpace(str);
  while (**str != '\0') {
    if (!isdigit(**str) && **str != '-') {
      std::cerr << "Invalid command!\n";
      throw std::exception();
    }
    nums.push_back(strtod(*str, &endp));
    if (*endp != ' ' && *endp != '\0') {
      std::cerr << "Invalid command!\n";
      throw std::exception();
    }
    *str = endp;
    skipSpace(str);
  }

  return nums;
}

// calculate function value at a set of centain points
double funSet::calFunValue(Expr * f, std::vector<double> pt) {
  for (size_t j = 0; j < pt.size(); j++) {
    if (f->getParams()[j].getExpr() != NULL) {
      delete f->getParams()[j].getExpr();
    }
    f->getParams()[j].getExpr() = new numExpr(pt[j]);
    if (f->exprType() == 3) {
      f->getExpr()->passInValues(f);
    }
  }
  return f->evaluate();
}

// helper methods for integration
// get integral ranges
std::vector<std::pair<double, double> > funSet::getIntRanges(std::vector<double> nums) {
  std::vector<std::pair<double, double> > intRanges;
  if (nums.size() % 2 == 1 ||
      nums.size() == 0) {  // numbers for ranges should appear in pairs greater than zero
    return intRanges;
  }
  intRanges = std::vector<std::pair<double, double> >(nums.size() / 2);
  for (size_t i = 0; i < intRanges.size(); i++) {
    intRanges[i].first = nums[i * 2];
    intRanges[i].second = nums[i * 2 + 1];
    if (intRanges[i].first >= intRanges[i].second) {
      return std::vector<std::pair<double, double> >(0);
    }
  }

  return intRanges;
}

// calculate integration
double funSet::calInt(Expr * f, std::vector<std::vector<double> > points) {
  double intRslt = 0;
  for (size_t i = 0; i < points.size(); i++) {
    intRslt += calFunValue(f, points[i]);
  }

  return intRslt;
}

// numerical integration
// use recursion to get the points of numerical integration
void funSet::getNumIntPts(std::vector<std::pair<double, double> > Ranges,
                          double width,
                          std::vector<double> pt,
                          std::vector<std::vector<double> > & points) {
  if (Ranges.size() == 0) {
    points.push_back(pt);
  }
  else {
    size_t numPts = round((Ranges[0].second - Ranges[0].first) /
                          width);  // number of points in range of a certain dimension
    for (size_t i = 0; i < numPts; i++) {
      std::vector<std::pair<double, double> > RG(Ranges);
      std::vector<double> PT(pt);
      RG.erase(RG.begin());
      PT.push_back(Ranges[0].first + i * width);
      getNumIntPts(RG, width, PT, points);
    }
  }
}

// calculate numerical integration
double funSet::calNumInt(Expr * f,
                         std::vector<std::vector<double> > points,
                         double width) {
  Expr * fCall = f->clone();
  double intRslt = 0;
  for (size_t i = 0; i < points.size(); i++) {
    // get local points
    std::vector<std::pair<double, double> > localRanges;
    std::vector<double> localPt;
    std::vector<std::vector<double> > localPoints;
    for (size_t j = 0; j < points[i].size(); j++) {
      localRanges.push_back(
          std::pair<double, double>(points[i][j], points[i][j] + width * 2));
    }
    getNumIntPts(localRanges, width, localPt, localPoints);

    // calculate function values and add to integral result
    intRslt += calInt(fCall, localPoints);
  }
  delete fCall;

  intRslt = intRslt / std::pow(2, f->getParams().size()) *
            std::pow(width, f->getParams().size());

  return intRslt;
}

// Monte Carlo integration
// get random points set for Monte Carlo integration
std::vector<std::vector<double> > funSet::getMcIntPts(
    std::vector<std::pair<double, double> > Ranges,
    int numPts) {
  std::vector<std::vector<double> > randPts;
  std::default_random_engine generator;
  for (int i = 0; i < numPts; i++) {
    std::vector<double> pt;
    for (size_t j = 0; j < Ranges.size(); j++) {
      std::uniform_real_distribution<double> distribution(Ranges[j].first,
                                                          Ranges[j].second);
      pt.push_back(distribution(generator));
    }
    randPts.push_back(pt);
  }

  return randPts;
}

// calculate Monte Carlo integration
double funSet::calMcInt(Expr * f,
                        std::vector<std::vector<double> > randPts,
                        std::vector<std::pair<double, double> > Ranges) {
  Expr * fCall = f->clone();

  // calculate function values
  double intRslt = calInt(fCall, randPts);
  delete fCall;

  intRslt /= randPts.size();

  for (size_t i = 0; i < Ranges.size(); i++) {
    intRslt *= (Ranges[i].second - Ranges[i].first);
  }

  return intRslt;
}

// helper methods for gradient ascent and descent
// get maximun number of steps
int funSet::getMaxNumStep(char ** str) {
  char * lastNum = *str + strlen(*str) - 1;
  while (*lastNum != ' ') {
    if (!isdigit(*lastNum)) {
      std::cerr << "Max number of steps can only be a positive integer!\n";
      throw std::exception();
    }
    lastNum--;
  }
  lastNum++;

  char * endp;
  int maxNumStep = strtol(lastNum, &endp, 10);
  *(lastNum - 1) = '\0';
  skiprSpace(str);

  return maxNumStep;
}

// calculate Euclidean distance
double funSet::calDis(std::vector<double> pt1, std::vector<double> pt2) {
  assert(pt1.size() == pt2.size());
  double dis = 0;
  for (size_t i = 0; i < pt1.size(); i++) {
    dis += std::pow(pt1[i] - pt2[i], 2);
  }

  return sqrt(dis);
}

// calculate gradient of a function at a set centain points
std::vector<double> funSet::gradient(Expr * f, std::vector<double> pt) {
  double h = 1e-5;
  std::vector<double> Df;
  for (size_t i = 0; i < pt.size(); i++) {
    std::vector<double> _pt(pt);
    _pt[i] += h;
    Df.push_back((calFunValue(f, _pt) - calFunValue(f, pt)) / h);
  }

  return Df;
}

// calculate gradient ascent or descent
std::vector<double> funSet::gradAstDst(Expr * f,
                                       std::vector<double> pcurr,
                                       double gamma,
                                       double convgDis,
                                       int maxNumStep,
                                       char op) {
  Expr * fCall = f->clone();

  std::vector<double> pnew(pcurr.size(), 0);
  for (int i = 0; i < maxNumStep; i++) {
    std::vector<double> Df = gradient(fCall, pcurr);
    for (size_t j = 0; j < Df.size(); j++) {
      if (op == '+') {
        pnew[j] = pcurr[j] + gamma * Df[j];
      }
      else {
        pnew[j] = pcurr[j] - gamma * Df[j];
      }
    }
    if (calDis(pnew, pcurr) < convgDis) {
      break;
    }
    pcurr = pnew;

    if (i == maxNumStep - 1) {
      std::cerr << "Iteration reaches maximun number of steps!\n";
      delete fCall;
      throw std::exception();
    }
  }
  delete fCall;

  return pnew;
}

// print gradient ascent or descent
void funSet::printGrad(std::vector<double> pt) {
  std::cout << '(';
  for (size_t i = 0; i < pt.size(); i++) {
    std::cout << pt[i];
    if (i != pt.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << ')' << '\n';
}

// look up function in funSet, return a pointer to the function if succeed, return NULL if fail
Expr * funSet::lookupFunction(std::string fname) {
  for (size_t i = 0; i < functions.size(); i++) {
    if (fname == functions[i]->getName()) {
      return functions[i];
    }
  }
  return NULL;
}

// add defined function to funSet
void funSet::addFunction(Expr * f) {
  functions.push_back(f->clone());
}

// destructor
funSet::~funSet() {
  for (size_t i = 0; i < functions.size(); i++) {
    delete functions[i];
  }
}

// default constructor
funSet::funSet() : functions(11, NULL) {
  std::vector<paramExpr> params;

  // 1-argument functions
  params.push_back(paramExpr("x"));
  functions[0] = new floorExpr(params);
  functions[1] = new ceilExpr(params);
  functions[2] = new roundExpr(params);

  // 2-argument functions
  params.push_back(paramExpr("y"));
  functions[3] = new plusExpr(params);
  functions[4] = new minusExpr(params);
  functions[5] = new timesExpr(params);
  functions[6] = new divExpr(params);
  functions[7] = new modExpr(params);
  functions[8] = new powExpr(params);
  functions[9] = new logbExpr(params);

  // 3-argument functions
  params.push_back(paramExpr("z"));
  functions[10] = new selectExpr(params);
}

// copy constructor
funSet::funSet(const funSet & rhs) : functions(rhs.functions.size(), NULL) {
  for (size_t i = 0; i < functions.size(); i++) {
    functions[i] = rhs.functions[i]->clone();
  }
}

// assignment operator
funSet & funSet::operator=(const funSet & rhs) {
  if (this != &rhs) {
    funSet temp(rhs);
    swap(functions, temp.functions);
  }
  return *this;
}

// method to operate define command
void funSet::define(char ** str) {
  // separate lhs string and rhs string
  char * lhs = *str;
  char * rhs = strchr(*str, '=');
  if (rhs == NULL) {
    std::cerr << "Improper format of function definition (could not find '=')!\n";
    throw std::exception();
  }
  *rhs = '\0';
  rhs++;

  // parse lhs of function definition
  std::string fname;
  std::vector<paramExpr> params = parseDefLhs(lhs, fname);
  // define function
  if (lookupFunction(fname) != NULL) {
    std::cerr << "Could not re-define an exist function!\n";
    throw std::exception();
  }
  Expr * f = new userDefFunExpr(fname, params);

  // parse rhs of function definition
  skipSpace(&rhs);
  skiprSpace(&rhs);
  if (countParen(&rhs) != 0) {
    std::cerr << "Parentheses should appear in pairs\n";
    delete f;
    throw std::exception();
  }
  f->getExpr() = parseDefRhs(&rhs, params);

  // add defined function to fSet
  if (f->getExpr() != NULL) {
    addFunction(f);
    printDef(f);
  }
  delete f;
}

// method to operate test command
void funSet::test(char ** str) {
  skipSpace(str);
  skiprSpace(str);

  // separate first and second string
  char * first = *str;
  char * second = *str + 1;
  if (*first != '(') {
    std::cerr << "Function call should start with '('!\n";
    return;
  }
  int numParen = 1;
  while (*second != '\0') {
    if (*second == '(') {
      numParen++;
    }
    else if (*second == ')') {
      numParen--;
    }
    second++;
    if (numParen == 0) {
      break;
    }
  }
  if (*second != ' ') {
    std::cerr << "Expect space after first arguement!\n";
    return;
  }
  *second = '\0';
  second++;
  skipSpace(&second);

  // parse
  Expr * argu1 = parseTest(&first);  // parse first argument
  char * endp;
  double argu2 = strtod(second, &endp);  // parse second argument
  if (argu1 != NULL && *endp == '\0') {
    printTest(argu1, argu2);
  }
  delete argu1;
}

// method to operate numint command
void funSet::numint(char ** str) {
  // get function name
  std::string fname = getFname(str);
  Expr * f = lookupFunction(fname);
  if (f == NULL) {
    std::cerr << "Could not find function " << fname << "!\n";
    throw std::exception();
  }

  // get width
  std::vector<double> nums = getNums(str);
  double width = nums[0];
  nums.erase(nums.begin());

  // get ranges
  std::vector<std::pair<double, double> > Ranges = getIntRanges(nums);
  if (Ranges.size() != f->getParams().size()) {
    std::cerr << "Invalid numint command!\n";
    throw std::exception();
  }
  for (size_t i = 0; i < Ranges.size(); i++) {
    double range = Ranges[i].second - Ranges[i].first;
    if ((range - std::floor(range / width) * width) > NUMTOL) {
      std::cerr << "Number of step size is not integer!\n";
      throw std::exception();
    }
  }

  // get integral points
  std::vector<double> pt;
  std::vector<std::vector<double> > points;
  getNumIntPts(Ranges, width, pt, points);

  // calculate integration
  double intRslt = calNumInt(f, points, width);

  std::cout << intRslt << "\n";
}

// method to operate mcint command
void funSet::mcint(char ** str) {
  // get function name
  std::string fname = getFname(str);
  Expr * f = lookupFunction(fname);
  if (f == NULL) {
    std::cerr << "Could not find function " << fname << "!\n";
    throw std::exception();
  }

  // get number of ramdom points
  skipSpace(str);
  char * endp;
  long numPts = strtol(*str, &endp, 10);
  if (*endp != ' ' || numPts <= 0) {
    std::cerr << "Invalid mcint command!\n";
    throw std::exception();
  }
  *str = endp;

  // get ranges
  std::vector<double> nums = getNums(str);
  std::vector<std::pair<double, double> > Ranges = getIntRanges(nums);
  if (Ranges.size() != f->getParams().size()) {
    std::cerr << "Invalid mcint command pair!\n";
    throw std::exception();
  }

  // get ramdom points
  std::vector<std::vector<double> > randPts = getMcIntPts(Ranges, numPts);

  // calculate integral
  double intRslt = calMcInt(f, randPts, Ranges);
  std::cout << intRslt << "\n";
}

// method to operate max and min command
void funSet::MaxMin(char ** str, char op) {
  // get function name
  std::string fname = getFname(str);
  Expr * f = lookupFunction(fname);
  if (f == NULL) {
    std::cerr << "Could not find function " << fname << "!\n";
    throw std::exception();
  }

  // get max number of step
  int maxNumStep = getMaxNumStep(str);

  // get other numbers
  std::vector<double> startPt = getNums(str);
  double gamma = startPt[0];
  double convgDis = startPt[1];
  startPt.erase(startPt.begin());
  startPt.erase(startPt.begin());
  if (startPt.size() != f->getParams().size()) {
    std::cerr << "Invalid numint command!\n";
    throw std::exception();
  }

  // calculate gradient ascent or descent
  std::vector<double> finalPt = gradAstDst(f, startPt, gamma, convgDis, maxNumStep, op);
  printGrad(finalPt);
}
