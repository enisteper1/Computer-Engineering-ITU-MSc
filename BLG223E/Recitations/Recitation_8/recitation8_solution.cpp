#include <iostream>  

using namespace std;

// a
enum MARK{X,O,B}; 
struct TTTboard {  
  enum MARK board[3][3] = {{B,B,B}, {B,B,B}, {B,B,B}}; 
};

// b
bool check_win(TTTboard a) {
  for (int i=0;i<3;i++){
    if (a.board[i][0] == a.board[i][1] && a.board[i][1] == a.board[i][2]) {   
      return true;
    }
    if (a.board[0][i] == a.board[1][i] && a.board[1][i] == a.board[2][i]) { 
      return true;
    }
  }

  if (a.board[0][0] == a.board[1][1] && a.board[1][1] == a.board[2][2]) {   
    return true;
  }
  if (a.board[0][2] == a.board[1][1] && a.board[1][1] == a.board[2][0]) {   
    return true;    
  }

  return false;
}

// c
bool check_tie(TTTboard a) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (a.board[i][j] == B) {
        return false;
      }
    }
  }
  return !check_win(a);
}

// d
bool check_future_tie(TTTboard a, MARK turn) {
  if (check_tie(a)) {
    return true;
  }

  MARK next_turn = turn == X ? O : X;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (a.board[i][j] == B) {
        a.board[i][j] = turn;
        if (check_future_tie(a, next_turn)) {
          char symb = turn == X ? 'X' : 'O';
          cout << symb << ":[" << i << "," << j << "]" <<endl;
          return true;
        }
        a.board[i][j] = B;
      }
    }
  }
  return false;
}


int main(int argc, char **argv) {
  TTTboard win_board = {{{O,X,X}, {O,O,X}, {B,B,X}}};
  TTTboard tie_board = {{{X,O,X}, {X,O,O}, {O,X,X}}};
  TTTboard ft_board = {{{X,O,X}, {X,O,O}, {B,B,X}}};

  bool win_result = check_win(win_board);
  bool tie_result = check_tie(tie_board);
  bool ft_result = check_future_tie(ft_board, O);

  cout << win_result << endl;
  cout << tie_result << endl;
  cout << ft_result << endl;

  return 0;
}