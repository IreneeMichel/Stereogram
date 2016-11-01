#include <vector>
using std::vector;
typedef unsigned int Uint32;
#include <iostream>
using namespace std;



class Monde {

public:
  // Constructeurs
  Monde(int LEVEL);
  void setScreenSize(  int x, int y,int distance);
  //Accesseurs et mutateurs
  void getImage(Uint32 * image) ;

  // Autres méthodes
  float move() ;
void turnLeft() ;
void turnRight() ;
void turnUp() ;
void turnDown() ;
bool inbloc(float *) ;
void moveForward() ;
void moveBack();
void switchMap() ;
void verifyPos();
void perdu();


private:
    vector<vector<vector<bool> > > dungeon ;
    bool map ;
    vector<int> theExit ;
    vector<vector<vector<float* > > > cross_positions ;
    int screen_height, screen_width ;
    int HEIGHT, WIDTH, DEPTH ;
    float pixel_size ;
    float position[3], direction[3], up[3] ;
    float  lateral[3]  ;
    void findDepth( int x, int y, int kmin, int &k) ;
    void projectZone(Uint32 * , int , int , int , int , int , int , int );
};
