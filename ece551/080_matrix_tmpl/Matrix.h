#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>

#include <cstdlib>
#include <iostream>
#include <vector>

//YOUR CODE GOES HERE!
template<typename T>
class Matrix {
 private:
  int numRows;
  int numColumns;
  std::vector<T> ** rows;

 public:
  Matrix() : numRows(0), numColumns(0), rows(NULL) {}
  Matrix(int r, int c) : numRows(r), numColumns(c), rows(new std::vector<T> *[r]) {
    for (int i = 0; i < r; i++) {
      rows[i] = new std::vector<T>(c);
    }
  }
  Matrix(const Matrix & rhs) :
      numRows(rhs.numRows),
      numColumns(rhs.numColumns),
      rows(new std::vector<T> *[numRows]) {
    for (int i = 0; i < numRows; i++) {
      rows[i] = new std::vector<T>(numColumns);
      *rows[i] = rhs[i];
    }
  }
  ~Matrix() {
    for (int i = 0; i < numRows; i++) {
      delete rows[i];
    }
    delete[] rows;
  }
  Matrix & operator=(const Matrix & rhs) {
    if (this != &rhs) {
      numRows = rhs.numRows;
      numColumns = rhs.numColumns;
      std::vector<T> ** tempRows = new std::vector<T> *[numRows];
      for (int i = 0; i < numRows; i++) {
        tempRows[i] = new std::vector<T>(numColumns);
        *tempRows[i] = *rhs.rows[i];
      }
      delete[] rows;
      rows = tempRows;
    }
    return *this;
  }
  int getRows() const { return numRows; }
  int getColumns() const { return numColumns; }
  const std::vector<T> & operator[](int index) const {
    assert(index < numRows && index >= 0);
    return *rows[index];
  }
  std::vector<T> & operator[](int index) {
    assert(index < numRows && index >= 0);
    return *rows[index];
  }
  bool operator==(const Matrix & rhs) const {
    if (numRows != rhs.numRows) {
      return 0;
    }
    if (numColumns != rhs.numColumns) {
      return 0;
    }
    for (int i = 0; i < numRows; i++) {
      if (!(*rows[i] == rhs[i])) {
        return 0;
      }
    }
    return 1;
  }
  Matrix operator+(const Matrix & rhs) const {
    assert(numRows == rhs.numRows && numRows >= 0 && rhs.numRows >= 0);
    assert(numColumns == rhs.numColumns && numColumns >= 0 && rhs.numColumns >= 0);
    Matrix ans(numRows, numColumns);
    for (int i = 0; i < rhs.numRows; i++) {
      for (int j = 0; j < rhs.numColumns; j++) {
        ans[i][j] = (*this)[i][j] + rhs[i][j];
      }
    }
    return ans;
  }
};

template<typename T>
std::ostream & operator<<(std::ostream & s, const std::vector<T> & rhs) {
  s << '{';
  for (uint i = 0; i < rhs.size(); i++) {
    s << rhs[i];
    if (i != rhs.size() - 1) {
      s << ", ";
    }
  }
  s << '}';
  return s;
}

template<typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
  s << "[ ";
  for (int i = 0; i < rhs.getRows(); i++) {
    s << rhs[i];
    if (i != rhs.getRows() - 1) {
      s << ",\n";
    }
  }

  s << " ]";
  return s;
}

#endif
