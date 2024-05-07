#include <bitset>
#include <iostream>
#include <vector>

const int bitsetSize = 9;

class Solution {
private:
  bool _wasChanged = true;
  int _size;
  std::vector<std::vector<std::bitset<bitsetSize>>> _possibilities;

  void _fillPossibilities() {
    std::bitset<bitsetSize> bitset;
    bitset.set();

    std::vector<std::bitset<bitsetSize>> row(_size, bitset);

    for (int i{0}; i < _size; ++i) {
      _possibilities.push_back(row);
    }
  }

  void _checkPossibilities(int row, int col,
                           std::vector<std::vector<char>>& board) {
    // check row
    auto tempBitset = _possibilities[row][col];
    for (int i{0}; i < _size; ++i) {
      if (board[row][i] != '.') {
        _possibilities[row][col].reset(static_cast<int>(board[row][i] - '1'));
      }
    }

    // check column
    for (int j{0}; j < _size; ++j) {
      if (board[j][col] != '.') {
        _possibilities[row][col].reset(static_cast<int>(board[j][col] - '1'));
      }
    }

    // check square
    for (int i = row - row % 3; i < row - row % 3 + 3; ++i) {
      for (int j = col - col % 3; j < col - col % 3 + 3; ++j) {
        if (board[i][j] != '.') {
          _possibilities[row][col].reset(static_cast<int>(board[i][j] - '1'));
        }
      }
    }

    if (tempBitset != _possibilities[row][col]) {
      _wasChanged = true;
    }

    if (_possibilities[row][col].count() == 1) {
      board[row][col] = '1' + _possibilities[row][col]._Find_first();
    }
  }

  void _checkUniquePossib(int row, int col,
                          std::vector<std::vector<char>>& board) {
    // check row
    auto tempBitset = _possibilities[row][col];
    for (int i{0}; i < _size; ++i) {
      if (i != col) {
        tempBitset &= tempBitset ^ _possibilities[row][i];
      }
    }

    if (tempBitset.count() == 1) {
      _wasChanged = true;
      _possibilities[row][col] = tempBitset;
      board[row][col] = '1' + tempBitset._Find_first();
      return;
    }

    // check column
    tempBitset = _possibilities[row][col];
    for (int j{0}; j < _size; ++j) {
      if (j != row) {
        tempBitset &= tempBitset ^ _possibilities[j][col];
      }
    }

    if (tempBitset.count() == 1) {
      _wasChanged = true;
      _possibilities[row][col] = tempBitset;
      board[row][col] = '1' + tempBitset._Find_first();
      return;
    }

    // check square
    tempBitset = _possibilities[row][col];
    for (int i = row - row % 3; i < row - row % 3 + 3; ++i) {
      for (int j = col - col % 3; j < col - col % 3 + 3; ++j) {
        if (!(i == row && j == col)) {
          tempBitset &= tempBitset ^ _possibilities[i][j];
        }
      }
    }

    if (tempBitset.count() == 1) {
      _wasChanged = true;
      _possibilities[row][col] = tempBitset;
      board[row][col] = '1' + tempBitset._Find_first();
    }
  }

// try to solve sudocu as a human
  bool _planA(std::vector<std::vector<char>>& board) {
    bool hasEmptyCells = true;

    while (hasEmptyCells) {
      _wasChanged = false;
      hasEmptyCells = false;

      for (int row{0}; row < _size; ++row) {
        for (int col{0}; col < _size; ++col) {
          if (board[row][col] == '.') {
            if (_possibilities[row][col].count() == 0) {
              return false;
            }
            hasEmptyCells = true;
            _checkPossibilities(row, col, board);
            _checkUniquePossib(row, col, board);
          }
        }
      }

      if (hasEmptyCells && !_wasChanged) {
        if (!_planB(board)) {
          return false;
        }
      }
    }

    return true;
  }

// try to guess next number
  bool _planB(std::vector<std::vector<char>>& board) {
    for (int row{0}; row < _size; ++row) {
      for (int col{0}; col < _size; ++col) {
        if (board[row][col] == '.') {
          for (int i{0}; i < bitsetSize; ++i) {
            if (_possibilities[row][col].test(i)) {
              board[row][col] = '1' + i;
              auto tempPossib = _possibilities;
              auto tempBoard = board;
              if (_planA(tempBoard)) {
                board = std::move(tempBoard);
                return true;
              }
              _possibilities = std::move(tempPossib);
            }
          }
          board[row][col] = '.';
          return false;
        }
      }
    }
    return false; // compiler requirements
  }

public:
  void solveSudoku(std::vector<std::vector<char>>& board) {
    // no parameter validation check

    _size = board.size();
    _fillPossibilities();

    for (int row{0}; row < _size; ++row) {
      for (int col{0}; col < _size; ++col) {
        if (board[row][col] != '.') {
          if (_possibilities[row][col].count() != 1) {
            _possibilities[row][col].reset().set(
                static_cast<int>(board[row][col] - '1'));
          }
        }
      }
    }

    if (!_planA(board)) {
      std::cout << "This Sudoku puzzle has no solution!" << std::endl;
    }
  }
};
