#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include "winbgi2.h"
#include <vector>
#include <list>
#include <map>
#include <time.h>
#include <windows.h>
#include <iostream>
const double width = 500, height = 500;
double dt = 0.1;
double R = 5;
double m = 1;
const int dx = 25;
class Box
{
    int ix,iy;
public:
    Box(int x=0, int y=0):ix(x),iy(y){}
    ~Box(){}
    void setX(int x=0){ix=x;}
    void setY(int y=0){iy=y;}
    int getX(){return ix;}
    int getY(){return iy;}
    bool operator<(const Box& b) const
    {
        if(ix<b.ix) return true;
        else if((ix == b.ix)&&(iy<b.iy)) return true;
        else return false;
    }
};

class Kulka
{
    //static int col;
    double x,y,vx,vy,Fx,Fy;
public:
    Kulka(double px=0,double py=0,double wx=0, double wy=0, double pFx=0, double pFy=0):x(px),y(py),vx(wx),vy(wy),Fx(pFx),Fy(pFy){}
    void przesun(double dt)
    {
        vx = vx + (Fx/m)*dt;
        vy = vy + (Fy/m)*dt;
        x = x + vx*dt;
        y = y + vy*dt;
        //if((x<=0)||(x>=width)) vx = -vx;
        //if((y<=0)||(y>=height)) vy = -vy;
        if((x<=0)||(x>=width)) Fx = -2*vx/dt;
        else Fx = 0;
        if((y<=0)||(y>=height)) Fy = -2*vy/dt;
        else Fy = 0;
    }
    double getY()
    {return y;}
    double getX()
    {return x;}
    double getVY()
    {return vy;}
    double getVX()
    {return vx;}
    void setFx(double F=0)
    {Fx=F;}
    void setFy(double F=0)
    {Fy=F;}
    void rysuj()
    {
        circle(x,height-y,R);
    }
    void set(double xx,double yy,double vxx,double vyy)
    {
        x = xx;
        y = yy;
        vx = vxx;
        vy = vyy;
    }
};

int main()
{

    int coll = 0;
    double dist = 0;
    typedef std::list<Kulka>::iterator list_iter;
    typedef std::list<Kulka> Kulki;
    typedef std::list<list_iter> LI;
    typedef std::map<Box,LI> Index;
    Index index;
    const int N = 50;
    Box box;
    int n;
    n=N;
    Kulki K;
    Kulka k(width/2,height/2,20,30),temp;
    double xtemp,ytemp,vxtemp,vytemp;
    srand(time(NULL));
    for(int i=0; i<N; i++)
    {
        xtemp = 500*(double)rand()/RAND_MAX;
        ytemp = 500*(double)rand()/RAND_MAX;
        vxtemp = 50*(double)rand()/RAND_MAX+10;
        vytemp = 50*(double)rand()/RAND_MAX+10;
        temp.set(xtemp,ytemp,vxtemp,vytemp);
        K.push_back(temp);
    }
	graphics(width, height);

    while(animate(15))
    {
        Index index;
        coll = 0;
        clear();
        for(list_iter iter = K.begin(); iter!=K.end(); iter++)
        {
            iter->przesun(dt);
            box.setX((int)(iter->getX()/dx));
            box.setY((int)(iter->getY()/dx));
            index[box].push_back(iter);

            /*LI::iterator list_box_start = index[box].begin();
            LI::iterator list_box_end = index[box].end();
            for(LI::iterator iter_in = list_box_start; iter_in != list_box_end; iter_in++)
            {
                if((*iter_in) != iter)
                {
                     dist = ((iter->getX())-((*iter_in)->getX()))*((iter->getX())-((*iter_in)->getX())) + ((iter->getY())-((*iter_in)->getY()))*((iter->getY())-((*iter_in)->getY()));
                     std::cout<<"DIST: "<<dist<<std::endl;
                }
                else
                    dist = 4*R*R;
                if(dist<4*R*R) coll++;
            }*/


            iter->rysuj();
            line(dx*box.getX(),height-dx*box.getY(),dx*box.getX()+dx,height-dx*box.getY());
            line(dx*box.getX(),height-dx*box.getY(),dx*box.getX(),height-(dx*box.getY()+dx));
            line(dx*box.getX()+dx,height-dx*box.getY(),dx*box.getX()+dx,height-(dx*box.getY()+dx));
            line(dx*box.getX(),height-(dx*box.getY()+dx),dx*box.getX()+dx,height-(dx*box.getY()+dx));
            //iterator++;
        }

        LI::iterator list_box_start = index[box].begin();
        LI::iterator list_box_end = index[box].end();
        for(Index::iterator iter = index.begin(); iter != index.end(); iter++)
        {

            for(LI::iterator in_iter = (iter->second).begin(); in_iter != (iter->second).end(); in_iter++)
            {
                for(LI::iterator in_in_iter = in_iter; in_in_iter != (iter->second).end(); in_in_iter++)
                 {
                     if(in_in_iter != in_iter)
                     {
                        dist = ((*in_in_iter)->getX()-(*in_iter)->getX())*((*in_in_iter)->getX()-(*in_iter)->getX()) + ((*in_in_iter)->getY()-(*in_iter)->getY())*((*in_in_iter)->getY()-(*in_iter)->getY());
                        if(dist<(4*R*R)) coll++;
                     }
                 }
            }
        }

        //system("pause");
        std::cout<<coll<<std::endl;

        //wait();
        n=N;
    }
	wait();
    //delete [] box;
}

