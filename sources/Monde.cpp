
#include "Monde.h"
#include "Color.h"
#include "Mole.h"
#include <cstdlib>
#include <math.h>
#include <omp.h>

//#include <windows.h>// sleep
//#include <unistd.h>
// ou
//#include <thread.hpp>


#define CALCGROUP 8
void crossProduct(float x[],float y[],float z[])
{
    float norm=sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]) ;
    for (int i =0;i<3;i++) x[i]=x[i]/norm ;
    float scal = x[0]*y[0]+x[1]*y[1]+x[2]*y[2] ;
    for (int i =0;i<3;i++) y[i]=y[i]-scal*x[i] ;
    norm=sqrt(y[0]*y[0]+y[1]*y[1]+y[2]*y[2]) ;
    for (int i =0;i<3;i++) y[i]=y[i]/norm ;
    z[0]=x[1]*y[2]-y[1]*x[2];
    z[1]=x[2]*y[0]-y[2]*x[0];
    z[2]=x[0]*y[1]-y[0]*x[1];
}

  Monde::Monde(int LEVEL)
{
   HEIGHT= WIDTH= DEPTH=LEVEL+5 ; // 2 pour les murs, 1 pour la sortie ; reste l entree et 1 couloir entre les 2
   vector<vector<vector<int> > > prepare ;
  dungeon.resize(WIDTH);
  prepare.resize(WIDTH) ;
   for (int i = 0; i < WIDTH; ++i) {
     dungeon[i].resize(HEIGHT);
     prepare[i].resize(HEIGHT);
     for (int j = 0; j < HEIGHT; ++j)
      {dungeon[i][j].resize(DEPTH);
       prepare[i][j].resize(DEPTH) ;
      for (int k = 0; k < DEPTH; ++k)
            {dungeon[i][j][k]=true;
            prepare[i][j][k]=0 ;
            }
        }
   }
     position[0]= 2.5 ;
     position[1]= 3.5 ;
     position[2]= 3.5 ;

     vector<Mole*> troup ;
     // use of pointer to avoid the call of copy constructor when push_back occurs ?

     cout << "begin of maze creation\n" ;
     troup.push_back(new Mole(prepare,1,2,3,3,1,troup)) ;
     theExit.push_back(WIDTH-1) ; theExit.push_back(HEIGHT/2) ;theExit.push_back(DEPTH/2) ;
     prepare[WIDTH-2][HEIGHT/2][DEPTH/2]=2 ; // devant sortie
     prepare[WIDTH-3][HEIGHT/2][DEPTH/2]=2 ;
     int jr=0 ;
     cout << "starting with "<<troup.size()<<" moles"<<endl ;
     while ((!troup.empty()) and (jr<2+WIDTH*2)) { jr++ ;
            vector<Mole*> troupcopy=troup ;
            for(vector<Mole*>::iterator i=troupcopy.begin();i!=troupcopy.end();i++) {
                    (*i)->move() ;
            }  ;
     } ;
     troup.push_back(new Mole(prepare,2,WIDTH-2,(int) (HEIGHT/2),(int) (DEPTH/2),-1,troup)) ;
     cout << "re starting with "<<troup.size()<<" moles"<<endl ;
     while ((!troup.empty()) and (jr<1200)) { jr++ ;
            vector<Mole*> troupcopy=troup ;
            for(vector<Mole*>::iterator i=troupcopy.begin();i!=troupcopy.end();i++) {
                 (*i)->move() ;
            }  ;
     } ;
     cout << "* end of maze creation\n" ;

     if (troup.size()>0) {cout <<" and it failed "<<endl; exit(0) ;} ;

    for (int i = 0; i < WIDTH; ++i)
       for (int j = 0; j < HEIGHT; ++j)
          for (int k = 0; k < DEPTH; ++k)
            dungeon[i][j][k]=(prepare[i][j][k]==0) ;

     if (dungeon[2][3][3]) { cout <<"origin in bloc 3\n" ; exit(1) ;};

     direction[0]= 0.6 ;
     direction[1]= -0.001 ;
     direction[2]= 0.4 ;
     up[0]=0. ;
     up[1]=0.9 ;
     up[2]=0.01 ;

      float ex[3] ;
      ex[0]=0. ; ex[1]=0. ; ex[2]=0. ;
       if (inbloc(ex)) { cout <<"origin in bloc \n" ; exit(1) ;} ;


     crossProduct(direction,up,lateral) ;

     map=false ;
}

void Monde::setScreenSize( int x, int y,int distance)
{
         // to do : last x positions do not need calculation
        this->screen_height = y ;
        this->screen_width = x ;
        // il faut definir toutes les intersections (3D array) dans le repere de l ecran
        // le repere est à la position position
        // et le repere est direction, up et lateral=direction^up
        pixel_size=0.119/distance ;
//        cross_positions.resize(x);
//        float pixel_pos[3]; //rpixel[3] ;
//        float reye[3] ;
        //float leye[3] ;
        //leye[0]=-0.3 ;
        //leye[1]=0. ;
        //leye[2]=-.06 ;
//        reye[0]=-0.3 ;
//        reye[1]=0. ;
//        reye[2]=.06 ;
//        unsigned int vecsize=MAXCROSS*x*y*4 ; // I put +1 for the pointer
        //cout << "trying memory allocation of vector of "<< (int) vecsize*4/1000000 <<" Mega Bytes \n" ;
        //float * bidon=new float[vecsize] ;

//        for (int i = 0; i < x; ++i) {
//           cross_positions[i].resize(y);
//           for (int j = 0; j < y; ++j) cross_positions[i][j].resize(MAXCROSS);
//        }
        //delete[] bidon  ;
//        cout << "memory allocation possible, program has a chance\n" ;
//        float ca,cb ;
         if ((x/2*pixel_size) > .4) {cout << "screen too big, wont enter the maze \n"<<x*pixel_size <<endl ; exit(1) ;}
//        pixel_pos[0]=0. ;  //rpixel[0]=pixel_pos[0] ;
//        //#pragma omp parallel for private(pixel_pos,rpixel)
//        for (int i = 0; i < x; ++i) {
//            //if (i>400) cout << "prepare " << i <<"/"<<x-1 << endl ;
//           for (int j = 0; j < y; ++j)
//           {
//                pixel_pos[2]=(i- x/2)*pixel_size ;
//                pixel_pos[1]=(-j+y/2)*pixel_size ; // il y a une inversion de l axe y de l image a faire quelque part, je l'ai ise ici
//                //cross_positions[i][j]={};
//                //vector<double*> ama ;
//                ca=(i- x/2-MAXCROSS/2) ;
//                if (ca<0) ca=(i- x/2+MAXCROSS/2) ;
//                if (ca>0) ca=0. ;
//                cb=(j-y/2-MAXCROSS/2) ;
//                if (cb<0) cb=(j-y/2+MAXCROSS/2) ;
//                if (cb>0) cb=0. ;
//                pixel_pos[0]=reye[0]*(ca*ca+cb*cb)/screen_height/screen_width;
//                //rpixel[0]=pixel_pos[0] ;
//                for (int k = 10; (k < MAXCROSS) ; ++k)
//                {
////                    rpixel[2]=pixel_pos[2]+k*pixel_size ;
////                    rpixel[1]=pixel_pos[1] ;
////                    float w=(leye[2]-reye[2])/(pixel_pos[2]-rpixel[2]) ;
////                    if (w!=w) {cout << " w is Nan   k="<<k<<" lpix=" <<pixel_pos[0]<<" "  <<pixel_pos[0]<<" "<<pixel_pos[0] ;
////                               cout <<"  rpixel="<<rpixel[0]<<" "<<rpixel[1]<<" "<<rpixel[2]<<"\n" ; throw 99 ;}
////                    if (w-1.<=0. ) { cout << " w is "<<w<<"  ; k="<<k<<"  \n i=" <<i<<" j="  <<j<<" x="<<x<<"  y="<<y<<endl ;
////                           cout <<"   lpix=" <<pixel_pos[0]<<" "  <<pixel_pos[1]<<" "<<pixel_pos[2] ;
////                              cout <<"  rpixel="<<rpixel[0]<<" "<<rpixel[1]<<" "<<rpixel[2]<<"\n" ; throw 99 ;}
////                     //cross_positions[i][j][k].resize(3);
////                    float *cp=new float[3] ;
////                    cp[0]=(w*pixel_pos[0]-leye[0])/(w-1.) ;
////                    cp[2]=(w*pixel_pos[2]-leye[2])/(w-1.) ;
////                    cp[1]=(w*pixel_pos[1])/(w-1) ;
////                    cross_positions[i][j][k]=cp ;
//                }
//           }
//
//  }
           //cout << " fin partie 1 \n" ;
}


inline bool Monde::inbloc(float * M)
{
     //cout<<"i" <<flush ;
    float real_pos[3] ;
    if (M[0]!=M[0] or M[1]!=M[1] or M[2]!=M[2] ) {
    cout << position[0] <<" "<< position[1]<<" " << position[2]<<" " << endl ;
    cout << "M"<<M[0] <<" "<< M[1]<<" " << M[2]<<" " << endl ; throw 104 ; }
    real_pos[0]=position[0]+M[0]*direction[0]+M[1]*up[0]+M[2]*lateral[0] ;
    real_pos[1]=position[1]+M[0]*direction[1]+M[1]*up[1]+M[2]*lateral[1] ;
    real_pos[2]=position[2]+M[0]*direction[2]+M[1]*up[2]+M[2]*lateral[2] ;
    if (real_pos[0]!=real_pos[0] or real_pos[1]!=real_pos[1] or real_pos[2]!=real_pos[2])
 {
    cout << "real pos "<<real_pos[0] <<" "<<real_pos[1]<<" " << real_pos[2]<<" " << endl ;
    cout << " -"<<direction[0]<< " " <<direction[1]<<" "<<direction[2]  ;
    cout << " -"<<up[0]<< " " <<up[1]<<" "<<up[2]  ;
    cout << " -"<<lateral[0]<< " " <<lateral[1]<<" "<<lateral[2] << endl ; throw 110 ; }
    if ((((int) real_pos[0])>=theExit[0])&&(((int) real_pos[1])==theExit[1])&&(((int) real_pos[2])==theExit[2]))
        return ((real_pos[1]-theExit[1]-0.5)*(real_pos[1]-theExit[1]-0.5)+(real_pos[2]-theExit[2]-0.5)*(real_pos[2]-theExit[2]-0.5)>0.2) ;
    if (real_pos[0]<0. or real_pos[1]<0. or real_pos[2]<0.
         or ((int) real_pos[0])>=WIDTH or ((int) real_pos[1])>=HEIGHT or ((int) real_pos[2])>=DEPTH  ) return true ;
    int s=dungeon[(int) real_pos[0] ][(int) real_pos[1]][(int) real_pos[2]] ;
    //cout<<"j" <<flush ;

    return s ;
}

void Monde::turnLeft()
{
 for (int i=0 ; i<3 ; i++) direction[i]=direction[i]-lateral[i]*0.15 ;
 crossProduct(direction,up,lateral) ;
 verifyPos() ;
}
void Monde::turnRight()
{
   for (int i=0 ; i<3 ; i++) direction[i]=direction[i]+lateral[i]*0.15 ;
    crossProduct(direction,up,lateral) ;
   verifyPos() ;
}
void Monde::turnUp()
{
  for (int i=0 ; i<3 ; i++) direction[i]=direction[i]+up[i]*0.15 ;
   crossProduct(direction,up,lateral) ;
    verifyPos() ;
}
void Monde::turnDown()
{
   for (int i=0 ; i<3 ; i++) direction[i]=direction[i]-up[i]*0.15 ;
   crossProduct(direction,up,lateral) ;
    verifyPos() ;
}
void Monde::moveForward()
{
   for (int i=0 ; i<3 ; i++) position[i]=position[i]+direction[i]*0.2 ;
    verifyPos() ;
}
void Monde::moveBack()
{
   for (int i=0 ; i<3 ; i++) position[i]=position[i]-direction[i]*0.2 ;
    verifyPos() ;
}
void Monde::switchMap() {map= (!map) ;}

void Monde::verifyPos()
{
  if ((((int) position[0])==theExit[0]-1) &&  (((int) position[1])==theExit[1]) && (((int) position[2])==theExit[2]))
   { cout << "\n victory : exit found \n" ; exit(0) ; }
      // pour survivre il faut reye, leye et les 4 angles  et le milieus hors blocs
        float reye[3] ;
        float leye[3] ;
        leye[0]=-0.3 ;
        leye[1]=0. ;
        leye[2]=-.06 ;
        reye[0]=-0.3 ;
        reye[1]=0. ;
        reye[2]=.06 ;
        if (inbloc(leye)) { cout <<"contact at rear leye\n" ; perdu();}
        if (inbloc(reye)) { cout <<"contact at rear reye\n" ; perdu();}
        //pixel_size=0.118/MAXCROSS ;
            leye[0]=0. ;
         for (int i = -10; i < screen_width+20; i=i+20)
           for (int j = -10; j < screen_height+20; j=j+20)
           {
                leye[2]=(i- screen_width/2)*pixel_size ;
                leye[1]=(j- screen_height/2)*pixel_size ;
                if (inbloc(leye)) { cout <<"contact at x="<<i<<"  y="<<j<<endl ; perdu();}
           }
   //cout << " ok\n" ;
   if (map) cout<< (int) position[0]<< "/"<<theExit[0]-1<<" "<<((int) position[1])<<"/"<<theExit[1]<<" "<< ((int) position[2])<<"/"<<theExit[2]<<endl ;
}

void Monde::perdu()
{
    cout <<" choc "<<endl ;
    for (int i=0 ; i<3 ; i++) position[i]=position[i]*0.5+(int((position[i]-direction[i]*0.05))+0.5)*0.5 ;

    float center[3] ; for (int i=0 ; i<3 ; i++) center[i]=0. ;
    if (inbloc(center)) { cout << "\n dead \n" ; exit(0) ;}
    else verifyPos() ;
}

//   inline void Monde::findDepth( int x, int y, int kmin, int &k)
//    {
//        //cout << "findDepth "<<x<<" "<<y<<" "<<kmin<<"\n" ;
//        //if (kmin<10) kmin=10 ;
//        if (inbloc(cross_positions[x][y][kmin])) {for (k=kmin+1 ; k>10 ; k--) if (not inbloc(cross_positions[x][y][k-1])) break ;}
//        else for ( k=kmin+1 ; k<MAXCROSS ; k=k+2) if (inbloc(cross_positions[x][y][k])) { if (inbloc(cross_positions[x][y][k-1])) k-- ; break ;} ;
//        if (k>=MAXCROSS) k=MAXCROSS-1 ;
//        if (k+x>=screen_width) k=0 ;
//        //cout << "fin findDepth "<<k<<"\n" ;
//        }

//    void Monde::projectZone(Uint32 * image,  int x, int y, int siz, int d1, int d2, int d3, int d4)
//    {
//          int dmin= std::max( (std::min(d1,d2)+std::min(d3,d4))/2,10) ;
//       //cout << "projectZone "<< x<<" "<<y<<" "<<siz<<" "<<" "<<d1<<" "<<d2<<" "<<d3<<" "<<d4<<"   min="<<dmin<<"\n" ;
//         int d5,d6,d7,d8,d9 ;
//          int misize=siz/2 ;
//          int index ;
//        //cout << "-"<<misize<< flush ;
//        findDepth(x+misize,y,dmin,d5) ;
//        findDepth(x,y+misize,dmin,d6) ;
//        findDepth(x+misize,y+misize,dmin,d7) ;
//        //cout << "a" << flush;
//        if (misize==1) {
//            index=x+y*screen_width ;
//            if (image[index]==0) image[index]=Color::randomColor() ;
//            //cout << "t"<< flush ;
//            image[index+d1]=image[index] ;
//            index=x+1+y*screen_width ;
//            if (image[index]==0) image[index]=Color::randomColor() ;
//            //cout << "b"<< flush ;
//            image[index+d5]=image[index] ;
//
//            index=x+(y+1)*screen_width ;
//            if (image[index]==0) image[index]=Color::randomColor() ;
//            //cout << "c" << flush;
//            image[index+d6]=image[index] ;
//
//            index=x+1+(y+1)*screen_width ;
//            if (image[index]==0) image[index]=Color::randomColor() ;
//            //cout << "d" << flush ;
//            image[index+d7]=image[index] ;
//        }
//        else {
//           //cout << "e" << flush ;
//             findDepth(x+siz,y+misize,dmin,d8) ; // peu utile
//            findDepth(x+misize,y+siz,dmin,d9) ; // peu utile
//           //cout << "g" << flush ;
//             projectZone(image,x,y,misize,d1,d6,d5,d7) ;
//            projectZone(image,x+misize,y,misize,d5,d7,d3,d8) ;
//           //cout << "h" << flush ;
//            projectZone(image,x,y+misize,misize,d6,d2,d7,d9) ;
//            projectZone(image,x+misize,y+misize,misize,d7,d9,d8,d4) ;
//            }
//            //cout << "f" << flush;
//           //cout << "fin projectZone\n" ;
//    }

void Monde::getImage(Uint32 * image)
{
    //cout << "get image \n" ;

    for (int i=0;i <screen_width*screen_height ; i++)
        image[i]=0 ;
    //cout << "get image init ok "<<screen_width<<"*"<<screen_height<<"\n" ;


   int k,plan,index;
    int minp,adp,maxp ;
    float pixel_pos[3],m[3] ;
    float real_pos[3] ;
    float alpha,alphamin ;
          //float reye[3] ;
        float leye[3] ;
   //float ca,cb;

        leye[0]=position[0]-0.5*direction[0]-.06*lateral[0] ;
        leye[1]=position[1]-0.5*direction[1]-.06*lateral[1] ;
        leye[2]=position[2]-0.5*direction[2]-.06*lateral[2] ;
     #pragma omp parallel for private(pixel_pos,real_pos,alphamin,alpha,plan,minp,maxp,adp,m)
      for (int j=0 ; j<screen_height;j=j+1)
     {
      //cout << " j"<<j <<" " <<flush ;

      for (int i=0;i <screen_width ; i=i+1)
      {
             //cout <<j<<" "<< i << " " ;
            pixel_pos[2]=(i-screen_width/2.)*pixel_size ;
            pixel_pos[1]=(-j+screen_height/2.)*pixel_size ; // il y a une inversion de l axe y de l image a faire quelque part, je l'ai mise ici
 //           ca=(screen_width- i/2-MAXCROSS/2) ;
//            if (ca<0) ca=(screen_width- i/2+MAXCROSS/2) ;
//            if (ca>0) ca=0. ;
//            cb=(j-j/2-MAXCROSS/2) ;
//            if (cb<0) cb=(j-j/2+MAXCROSS/2) ;
//            if (cb>0) cb=0. ;
//            pixel_pos[0]=-0.3*(ca*ca+cb*cb)/screen_height/screen_width;
            pixel_pos[0]=0. ;
            real_pos[0]=position[0]+pixel_pos[0]*direction[0]+pixel_pos[1]*up[0]+pixel_pos[2]*lateral[0] ;
            real_pos[1]=position[1]+pixel_pos[0]*direction[1]+pixel_pos[1]*up[1]+pixel_pos[2]*lateral[1] ;
            real_pos[2]=position[2]+pixel_pos[0]*direction[2]+pixel_pos[1]*up[2]+pixel_pos[2]*lateral[2] ;
            alphamin=1. ;
             alpha=0. ;
           if (real_pos[0]>leye[0]) {minp=int(position[0]+1.) ; maxp=WIDTH ; adp=1;}
            else  {minp=int(position[0]) ; maxp=0 ; adp=-1;};
           for (plan=minp;(plan!=maxp) and (alpha<alphamin);plan+=adp)
            {
                alpha=(real_pos[0]-plan)/(leye[0]-plan) ;
                //cout << alpha << endl ;
               m[1]=(real_pos[1]-alpha*leye[1])/(1.-alpha) ;
               //cout << m[1] << endl ;
                if (m[1]<=1. or m[1]>=HEIGHT-1 ) break ; // {cout << " out downup " ; break ;}
                m[2]=(real_pos[2]-alpha*leye[2])/(1.-alpha) ;
              //cout << m[2] << endl ;
                 if (m[2]<=1. or m[2]>=DEPTH-1 ) break ; //{cout << " out rleft " ; break ;}
                //cout << " for "<<plan<<":"<<plan-1*(adp<0)<<" "<<int(m[1])<<" "<<int(m[2]) ;
                if (adp>0 and plan==theExit[0] && int(m[1])==theExit[1] && int(m[2])==theExit[2] )  {
                    if ((m[1]-theExit[1]-0.5)*(m[1]-theExit[1]-0.5)+(m[2]-theExit[2]-0.5)*(m[2]-theExit[2]-0.5)>0.1)  break ;
                    else {alpha= 1.  ;  break ;}}
                else if ( dungeon[plan-1*(adp<0)][int(m[1])][int(m[2])]) break ; //{ cout << " blocx "; break ;}
                //cout << " no contact; " ;
            }
            alphamin=min(alpha,alphamin) ;
            //cout << " alphax "<< alphamin ;
            if (real_pos[1]>leye[1]) {minp=int(position[1]+1) ; maxp=HEIGHT ; adp=1;}
            else  {minp=int(position[1]) ; maxp=0 ; adp=-1;};
            alpha=0. ;
            //cout << "b "<<minx<<" "<< maxx<<" "<< adx << endl ;
           for (plan=minp;(plan!=maxp) and (alpha<alphamin);plan+=adp)
            {
                alpha=(real_pos[1]-plan)/(leye[1]-plan) ;
                //cout << alpha << endl ;
               m[0]=(real_pos[0]-alpha*leye[0])/(1.-alpha) ;
               //cout << m[1] << endl ;
                if (m[0]<=1. or m[0]>=WIDTH-1 ) break ; //{ cout << " out x ++ " ; break ;} ;
                m[2]=(real_pos[2]-alpha*leye[2])/(1.-alpha) ;
              //cout << m[2] << endl ;
                 if (m[2]<=1. or m[2]>=DEPTH-1 ) break ; //{ cout << " out up ++ " ; break ;} ;
                //cout << planx<<" "<<planx-1*(adx<0)<<" "<<int(m[1])<<" "<<int(m[2])<< " " <<dungeon[planx-1*(adx<0)][int(m[1])][int(m[2])]<<endl ;
                if ( dungeon[int(m[0])][plan-1*(adp<0)][int(m[2])]) break ; //{ cout << " blocy";break ;}
                //cout <<" escape 1 " ;
            }
            //if (alpha<alphamin) {cout << "+>" << alpha ; exit(1) ;}
            alphamin=min(alpha,alphamin) ;

            if (real_pos[2]>leye[2]) {minp=int(position[2]+1) ; maxp=DEPTH ; adp=1;}
            else  {minp=int(position[2]) ; maxp=0 ; adp=-1;};
            alpha=0. ;
            //cout << "b "<<minx<<" "<< maxx<<" "<< adx << endl ;
           for (plan=minp;(plan!=maxp) and (alpha<alphamin);plan+=adp)
            {
                alpha=(real_pos[2]-plan)/(leye[2]-plan) ;
                //cout << alpha << endl ;
               m[1]=(real_pos[1]-alpha*leye[1])/(1.-alpha) ;
               //cout << m[1] << endl ;
                if (m[1]<=1. or m[1]>=HEIGHT-1 ) break ; //{ cout << " out up -- " ; break ;}
                m[0]=(real_pos[0]-alpha*leye[0])/(1.-alpha) ;
              //cout << m[2] << endl ;
                if (m[0]<=1. or m[0]>=WIDTH-1 )  break ; //{ cout << " out x -- " ; break ;}
                //cout << planx<<" "<<planx-1*(adx<0)<<" "<<int(m[1])<<" "<<int(m[2])<< " " <<dungeon[planx-1*(adx<0)][int(m[1])][int(m[2])]<<endl ;
                if ( dungeon[int(m[0])][int(m[1])][plan-1*(adp<0)]) break ; //{ cout << " blocy"; break  ;}
                //cout <<" escape 2 " ;
            }
            //if (alpha<alphamin) {cout <<"->" << alpha ;}
            alphamin=min(alpha,alphamin) ;
                //cout << "c" ;
            k=int(alphamin*0.12/pixel_size+0.6) ;
            //cout << " k " << k << endl;

            index=i+j*screen_width ;
            if (image[index]==0) image[index]=Color::randomColor() ;

            if (i+k<screen_width)
              image[index+k]=image[index] ;
      }}
//    int index ;
//     for (int j=0 ; j<screen_height;j=j+1)
//     {
//         cout << j << endl ;
//
//      for (int i=0;i <screen_width ; i=i+1)
//      {
//            // cout << i ;
//            findDepth(i,j,10,k1) ;
//            index=i+j*screen_width ;
//            if (image[index]==0) image[index]=Color::randomColor() ;
//            if (i==1) { image[index]=(255<<16) ; cout << "i=1 ; index=" <<index <<endl ; };
//            if (i==10) { image[index]=(255) ; cout << "i=10 ; index=" <<index <<endl ; };
//            if (i==30) { image[index]=(255<<8) ; cout << "i=30 ; index=" <<index <<endl ; };
//            if (i+k1<screen_width)
//              image[index+k1]=image[index] ;
//      }}
//    int k1,k2,k3,k4 ;
//    k1=0 ; k2=0 ;
//  #pragma omp parallel for private(k1,k2,k3,k4)
//     for (int j=0 ; j<screen_height-CALCGROUP;j=j+CALCGROUP) {
//            //int ID = omp_get_thread_num();
//            //cout << "j "<<j<<" done by thread "<<ID << endl ;
//    k3=0; k4=0 ;
//      for (int i=0;i <screen_width-CALCGROUP-5 ; i=i+CALCGROUP)
//      {
//         //cout << " " << i  ;
//        findDepth(i,j,10,k1) ;
//        findDepth(i,j+CALCGROUP,10,k2) ; //cout << "ij "<<i<<" "<<j<<"found "<<k1  <<" "<< k2 <<endl ;
//        if (k3==0) findDepth(i+CALCGROUP,j,10,k3) ;
//        if (k4==0) findDepth(i+CALCGROUP,j+CALCGROUP,10,k4) ;
//        projectZone(image,i,j,CALCGROUP,k1,k2,k3,k4) ;
//        k3=k1 ;
//        k4=k2 ;
//        //cout << " " << i << " ok * ";
//        }
//        //cout << " * \n";
//       for (int i=screen_width-18+j*screen_width;i <screen_width+j*screen_width ; i++)
//        {
//         if (image[i]==0) image[i]=Color::randomColor() ;
//        }
//     }
 //cout << "fin get image \n" ;

 return ;
}

float Monde::move()
{
  return rand() % 1000000;
}
