#ifndef MAZE_H_INCLUDED
#define MAZE_H_INCLUDED

#include <vector>
using std::vector;
using namespace std;
#include <algorithm>    // std::remove
#include <iostream>
class Mole {

public:
  // Constructeurs
  Mole( vector<vector<vector<int> > > &dungeon,int t,int x,int y,int z,int dir,vector<Mole*> &troup);
  ~Mole() ;
  void move() ;
  //bool operator==(const Mole b) {std::cout << " compare "<<&b<<"=="<<this<<" " ;  return (&b==this) ;} ;

private:
    vector<vector<vector<int> > >  *dungeon ;
    int tribe ;
    int i,j,k ;
    int direction ;
    vector<Mole*> *troup ;
    void changeDirection() ;
};

#endif // MAZE_H_INCLUDED
