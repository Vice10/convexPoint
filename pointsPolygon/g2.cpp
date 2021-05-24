#include "g2.h"

Point::Point():x_(0.), y_(0.) {}

Point &Point::operator=(const Point &r) {
    if(this == &r)
        return *this;
    x_ = r.x_;
    y_ = r.y_;
    return *this;
}

bool Point::operator==(Point r) {
    if(x_ == r.x_ && y_ == r.y_)
        return true;
    return false;
}

bool Point::operator!=(Point r) {
    if(x_ != r.x_ || y_ != r.y_)
        return true;
    return false;
}

Point::Point(double x, double y): x_(x), y_(y) {}

double Point::getCoordsX() {
    return x_;
}

double Point::getCoordsY() {
    return y_;
}


//----


DirLine::DirLine(Point p1, Point p2):p1_(p1), p2_(p2) {
    if(p1_.getCoordsX() != p2_.getCoordsX() || p1_.getCoordsY() != p2_.getCoordsY())
        isLine_ = true;
}

bool DirLine::operator==(DirLine line) {
    if (p1_.getCoordsX() == line.getPoint1().getCoordsX()
        && p1_.getCoordsY() == line.getPoint1().getCoordsY()
        && p2_.getCoordsX() == line.getPoint2().getCoordsX()
        && p2_.getCoordsY() == line.getPoint2().getCoordsY())
        return true;
    return false;
}

bool DirLine::operator!=(DirLine line) {
    if (p1_.getCoordsX() != line.getPoint1().getCoordsX()
        || p1_.getCoordsY() != line.getPoint1().getCoordsY()
        || p2_.getCoordsX() != line.getPoint2().getCoordsX()
        || p2_.getCoordsY() != line.getPoint2().getCoordsY())
        return true;
}


Point DirLine::getPoint1() {
    return p1_;
}

Point DirLine::getPoint2() {
    return p2_;
}

int DirLine::dev(Point workPoint) {
    double d = ( (p2_.getCoordsX() - p1_.getCoordsX()) * (workPoint.getCoordsY() - p1_.getCoordsY())
                 - (workPoint.getCoordsX() -  p1_.getCoordsX()) * (p2_.getCoordsY() - p1_.getCoordsY()) );
    if (d > 0)
        return 1;
    else if(d < 0)
        return -1;
    return 0;
}

bool DirLine::isLine() {
    return isLine_;
}


//----


Segment::Segment(Point p1, Point p2):p1_(p1), p2_(p2) {
    if (p1.getCoordsX() >= p2.getCoordsX()) {
        max_x_coord = p1.getCoordsX();
        min_x_coord = p2.getCoordsX();
    }
    else {
        max_x_coord = p2.getCoordsX();
        min_x_coord = p1.getCoordsX();
    }
}

bool Segment::operator==(Segment seg) {
    if (p1_.getCoordsX() == seg.getPoint1().getCoordsX()
        && p1_.getCoordsY() == seg.getPoint1().getCoordsY()
        && p2_.getCoordsX() == seg.getPoint2().getCoordsX()
        && p2_.getCoordsY() == seg.getPoint2().getCoordsY())
        return true;
    return false;
}

bool Segment::operator!=(Segment seg) {
    if (p1_.getCoordsX() != seg.getPoint1().getCoordsX()
        || p1_.getCoordsY() != seg.getPoint1().getCoordsY()
        || p2_.getCoordsX() != seg.getPoint2().getCoordsX()
        || p2_.getCoordsY() != seg.getPoint2().getCoordsY())
        return true;
    return false;
}


Point Segment::getPoint1() {
    return p1_;
}

Point Segment::getPoint2() {
    return p2_;
}

double Segment::distP1(Point p) {
    return sqrt((p.getCoordsX() - p1_.getCoordsX())*(p.getCoordsX() - p1_.getCoordsX()) +
                (p.getCoordsY() - p1_.getCoordsY())*(p.getCoordsY() - p1_.getCoordsY()));
}

double Segment::distP2(Point p) {
    return sqrt((p.getCoordsX() - p2_.getCoordsX())*(p.getCoordsX() - p2_.getCoordsX()) +
                (p.getCoordsY() - p2_.getCoordsY())*(p.getCoordsY() - p2_.getCoordsY()));;
}

bool Segment::onSeg(Point p) {
    if (distP1(p) + distP2(p) == distP1(p2_))
        return true;
    return false;
}

bool Segment::on_the_rightX(Point checkPoint)
{
    if (checkPoint.getCoordsX() > max_x_coord)
        return true;
    return false;
}

bool Segment::on_the_leftX(Point checkPoint)
{
    if (checkPoint.getCoordsX() < min_x_coord)
        return true;
    return false;
}


//----


PointArray::PointArray():size_(0), capacity_(0), p(new Point[0]){}

PointArray::~PointArray(){delete []p;}

bool PointArray::resize() {
    if(capacity_ == 0){
        size_ = N;
    }
    else
        size_ = size_ * 2;
    Point* buff;
    try {
        buff = new Point[size_];
    }
    catch (bad_array_new_length &e) {
        cout << e.what() << '\n';
        cout << "Exception in resize\n";
        return false;
    }
    for (int i = 0; i < capacity_; ++i) {
        buff[i] = p[i];
    }
    delete []p;
    p = buff;
    return true;
}

void PointArray::addCapacity() {
    capacity_++;
}

void PointArray::pushBack(double x, double y) {
    if(capacity_ == size_ - 1 || capacity_ == 0)
        if(!resize())
            throw 100;
    p[capacity()] = Point(x, y);
}
/*void PointArray::setWorkPoint(double x, double y) {
    workPoint = Point(x, y);
}*/
unsigned PointArray::capacity() const {
    return capacity_;
}
unsigned PointArray::size() const {
    return size_;
}

double PointArray::pointCoordX(int i) {
    return p[i].getCoordsX();
}

double PointArray::pointCoordY(int i) {
    return p[i].getCoordsY();
}

int PointArray::wn_PnPoly(Point workPoint)const {  // -1 on // 1 in // 0 out
    int wn = 0;    // the  winding number counter
    // loop through all edges of the polygon
    for (int i=0; i < capacity(); i++) {   // edge from V[i] to  V[i+1]
        if(i == capacity() - 1){
            DirLine line{p[i], p[0]};
            Segment seg{p[i], p[0]};
            if(seg.onSeg(workPoint))
                return -1;
            if(line.isLine()){
                if (p[i].getCoordsY() <= workPoint.getCoordsY()) {          // start y <= P.y
                    if (p[0].getCoordsY()  > workPoint.getCoordsY())      // an upward crossing
                        if (line.dev(workPoint) > 0)  // P left of  edge
                            ++wn;            // have  a valid up intersect
                }
                else {                        // start y > P.y (no test needed)
                    if (p[0].getCoordsY()  <= workPoint.getCoordsY())     // a downward crossing
                        if (line.dev(workPoint) < 0)  // P right of  edge
                            --wn;            // have  a valid down intersect
                }
            }
        }
        else{
            DirLine line{p[i], p[i + 1]};
            Segment seg{p[i], p[i+1]};
            if(seg.onSeg(workPoint))
                return -1;
            if(line.isLine()){
                if (p[i].getCoordsY() <= workPoint.getCoordsY()) {          // start y <= P.y
                    if (p[i+1].getCoordsY()  > workPoint.getCoordsY())      // an upward crossing
                        if (line.dev(workPoint) > 0)  // P left of  edge
                            ++wn;            // have  a valid up intersect
                }
                else {                        // start y > P.y (no test needed)
                    if (p[i+1].getCoordsY()  <= workPoint.getCoordsY())     // a downward crossing
                        if (line.dev(workPoint) < 0)  // P right of  edge
                            --wn;            // have  a valid down intersect
                }
            }
        }
    }
    return wn;
}
Point PointArray::getWorkPoint(int i) const {
    return p[i];
}
int PointArray::cn_PnPoly(Point workPoint) const {

    int cn = 0;    // the  crossing number counter
    // loop through all edges of the polygon
    for (int i=0; i < capacity(); i++) {    // edge from V[i]  to V[i+1]
        if(i == capacity() - 1){
            Segment seg{p[i], p[0]};
            if(seg.onSeg(workPoint))
                return -1;
            if (((p[i].getCoordsY() <= workPoint.getCoordsY()) && (p[0].getCoordsY() > workPoint.getCoordsY()))     // an upward crossing
                || ((p[i].getCoordsY() > workPoint.getCoordsY()) && (p[0].getCoordsY() <=  workPoint.getCoordsY()))) { // a downward crossing
                // compute  the actual edge-ray intersect x-coordinate
                double vt = (workPoint.getCoordsY()  - p[i].getCoordsY()) / (p[0].getCoordsY() - p[i].getCoordsY());
                if (workPoint.getCoordsX() <  p[i].getCoordsX() + vt * (p[0].getCoordsX() - p[i].getCoordsX())) // P.x < intersect
                    ++cn;   // a valid crossing of y=P.y right of P.x
            }
        } else {
            Segment seg{p[i], p[0]};
            if(seg.onSeg(workPoint))
                return -1;
            if (((p[i].getCoordsY() <= workPoint.getCoordsY()) &&
                 (p[i + 1].getCoordsY() > workPoint.getCoordsY()))     // an upward crossing
                || ((p[i].getCoordsY() > workPoint.getCoordsY()) &&
                    (p[i + 1].getCoordsY() <= workPoint.getCoordsY()))) { // a downward crossing
                // compute  the actual edge-ray intersect x-coordinate
                double vt = (workPoint.getCoordsY() - p[i].getCoordsY()) / (p[i + 1].getCoordsY() - p[i].getCoordsY());
                if (workPoint.getCoordsX() <
                    p[i].getCoordsX() + vt * (p[i + 1].getCoordsX() - p[i].getCoordsX())) // P.x < intersect
                    ++cn;   // a valid crossing of y=P.y right of P.x
            }
        }
    }
    return (cn&1);    // 0 if even (out), and 1 if  odd (inFile)
}

