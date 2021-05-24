#pragma once
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

class Point {
public:
    Point();
    Point(double, double);
    Point& operator=(const Point&);
    bool operator==(Point);
    bool operator!=(Point);
    double getCoordsX();
    double getCoordsY();
private:
    double x_;
    double y_;
};

class PointArray {
public:
    PointArray(const PointArray&) = delete;
    PointArray &operator=(const PointArray&) = delete;
    PointArray();
    ~PointArray();
    unsigned size()const;
    unsigned capacity()const;
    //string inputFilePath()const;
    void addCapacity();
    void pushBack(double, double);
    //void setWorkPoint(double, double);
    double pointCoordX(int);
    double pointCoordY(int);
    int wn_PnPoly(Point workPoint)const ;
    int cn_PnPoly( Point workPoint)const;
    Point getWorkPoint(int)const;

private:
    //Point workPoint;
    Point* p = nullptr;
    unsigned size_;
    unsigned capacity_;
    bool resize();
    static const unsigned N = 100;
    //string inputFilePath_;
};

class DirLine {
public:
    DirLine(Point, Point);
    DirLine();
    bool operator==(DirLine);
    bool operator!=(DirLine);
    Point getPoint1();
    Point getPoint2();
    int dev(Point);
    bool isLine();

private:
    Point p1_{0., 0.};
    Point p2_{0., 0.};
    bool isLine_ = false;
};

class Segment {
public:
    Segment();
    Segment(Point, Point);
    bool operator==(Segment);
    bool operator!=(Segment);
    Point getPoint1();
    Point getPoint2();
    double distP1(Point);
    double distP2(Point);
    bool onSeg(Point);
    bool on_the_rightX(Point);
    bool on_the_leftX(Point);

private:
    Point p1_, p2_;
    double max_x_coord;
    double min_x_coord;
};
