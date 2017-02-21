
#include "Mole.h"
using std::vector;

#include <stdlib.h>     /* abs */

Mole::Mole( vector<vector<vector<int> > > &xdungeon,int t,int x,int y , int z,int dir,vector<Mole*> &xtroup) :
         tribe(t),i(x),j(y),k(z),direction(dir)
{
    troup=&xtroup ;
    dungeon=&xdungeon ;
  //cout << "troup size " << troup->size() << endl ;
  //cout << "mole "<<this<<" birth at" << x << " "<<y<<" "<<z<<"  tribe="<<tribe << endl ;
}

void Mole::move()
{
    //cout << "move from "  <<i << " "<<j<<" "<<k<< endl ;
    (*dungeon)[i][j][k]=tribe ;
    int width=(*dungeon).size() ;
    int hight=(*dungeon)[0].size();
    int depth=(*dungeon)[0][0].size() ;
    // poss
    int pi,pj,pk ;
    pi=rand()%20 ; // pour changer le premier rnd pour la suite
    if (abs(direction)==1) pi=i+direction  ;
    else pi=i ;
    if (abs(direction)==2) pj=j+direction/2  ;
    else pj=j ;
    if (abs(direction)==3) pk=k+direction/3 ;
    else pk=k ;
    //cout << "testing "  <<pi << " "<<pj<<" "<<pk<< endl ;

    if ((tribe==2) and ((*dungeon)[pi][pj][pk]==1)) { cout << "exit exists\n" ; delete this ;  return ;}

    if ((pi==0) or (pi==width-1)) {changeDirection() ; return ;} ;
    if ((pj==0) or (pj==hight-1)) {changeDirection() ; return ;} ;
    if ((pk==0) or (pk==depth-1)) {changeDirection() ; return ;} ;

    //cout << "calculating "  <<pi << " "<<pj<<" "<<pk<< endl ;
    int malus=0 ;
    for (int x=pi-1 ; x<pi+2 ; x++)
        for (int y=pj-1 ; y<pj+2 ; y++)
            for (int z=pk-1 ; z<pk+2 ; z++)
                malus+=(*dungeon)[x][y][z] ;
    for (int x=pi-1 ; x<pi+2 ; x++) malus-=(*dungeon)[x][pj][pk] ;
    for (int y=pj-1 ; y<pj+2 ; y++) malus-=(*dungeon)[pi][y][pk] ;
    for (int z=pk-1 ; z<pk+2 ; z++) malus-=(*dungeon)[pi][pj][z] ;

    if (abs(pi-width/2)<abs(i-width/2)) malus+=1 ;
    if (abs(pj-hight/2)<abs(j-hight/2)) malus+=1 ;
    if (abs(pk-depth/2)<abs(k-depth/2)) malus+=1 ;

    if ((pi==1) or (pi==width-2)) malus+=8 ;
    if ((pj==1) or (pj==hight-2)) malus+=8 ;
    if ((pk==1) or (pk==depth-2)) malus+=8 ;
    if ((pi==2 and i!=1) or (pi==width-3 and i!=width-2)) malus+=2 ;
    if ((pj==2 and j!=1) or (pj==hight-3 and j!=hight-2)) malus+=2 ;
    if ((pk==2 and k!=1) or (pk==depth-3 and k!=depth-2)) malus+=2 ;

    // dans le vide malus = 27-9
    // en bout de couloir malus = 2-4
    //cout << " malus "<<malus << endl ;
    if (tribe==2) malus=malus/2-2 ;
    int rnd=rand()%22-malus ;
    if (rnd+1<0 and tribe!=2)  { delete this ; return ;}
    else if (rnd<3) { changeDirection() ; return ;}
    else if ((rnd>18) and (tribe!=2)) { Mole *m=new Mole(*dungeon,tribe,i,j,k,direction,*troup) ; troup->push_back(m); cout << troup->size()<< " moles"<<endl  ; m->changeDirection() ; }

    //cout << tribe << " advance to "<<pi<<" "<<pj<<" "<<pk << endl ;
    i=pi ; j=pj ; k=pk ;

}

void Mole::changeDirection()
{
   //cout << tribe <<" changeDirection\n" ;
   int previous=direction ;
   while ((direction==0) or (direction==previous)) direction=rand()%7-3 ;
}

Mole::~Mole() {
  //cout << " delete : " << this << " in "<< troup->size()  ;
 troup->erase(remove(troup->begin(), troup->end(), this), troup->end());
 //cout << " " <<troup->size() << " moles"<< endl ;
 }
