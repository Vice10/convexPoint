#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "g2.h"
using namespace std;
typedef unsigned int uint;
int pointSit(Point workPoint);
int segIdx(const vector<pair<double, double>>& chain, double workPointX);
uint getMaxElemIdx(const vector<pair<double, double>>& ch);
bool inputPoints(ifstream& fin);
void displayPointSitRes(int res, Point wp);
void writePointSitFile(ofstream& fout, int res, Point wp);

vector<pair<double, double>>ch;
vector<pair<double, double>>topCh;
vector<pair<double, double>>botCh;
uint topChElemCount = 0;
uint botChElemCount = 0;
vector<pair<double, double>>workPoints;
uint totalIn = 0, totalOut = 0, totalOn = 0;
int main()
{
    string fname;
    cout << "Enter input file name:\n";
    cin >> fname;
    ifstream fin(fname, ios::in);
    if (!inputPoints(fin)) {
        cout << "Failed to open the input file. Terminating...\n";
        return 2;
    }
    topChElemCount = getMaxElemIdx(ch) + 1;
    botChElemCount = ch.size() - topChElemCount + 2;
    for (uint i = 0; i < topChElemCount; i++)
        topCh.push_back(ch[i]);
    for (uint i = topChElemCount - 1; i < ch.size(); i++)
        botCh.push_back(ch[i]);
    botCh.push_back(ch[0]);
    reverse(botCh.begin(), botCh.end());

    ofstream fout("results.txt", ios::out);
    for (uint i = 0; i < workPoints.size(); i++) {
        Point wp{ workPoints[i].first, workPoints[i].second };
        int res = pointSit(wp);
        //displayPointSitRes(res, wp);
        writePointSitFile(fout, res, wp);
    }
    fout.close();
    cout << "Total\n--IN: " << totalIn << "\n--OUT: " << totalOut << "\n--ON: " << totalOn << "\n\n";
    double x, y;
    cout << "Enter work points:\n";
    while (true) {
        if (!(cin >> x >> y))
            return 1;
        Point wp{ x, y };
        int res = pointSit(wp);
        displayPointSitRes(res, wp);
    }
    return 0;
}

int pointSit(Point workPoint)
{   // -1 on // 1 in // 0 out
    int topIdx = segIdx(topCh, workPoint.getCoordsX());
    int botIdx = segIdx(botCh, workPoint.getCoordsX());
    if (topIdx == topChElemCount - 1 || topIdx == -1)
        return 0;
    Point topP1{ topCh[topIdx].first, topCh[topIdx].second };
    Point topP2{ topCh[topIdx + 1].first, topCh[topIdx + 1].second };
    Point botP1{ botCh[botIdx].first, botCh[botIdx].second };
    Point botP2{ botCh[botIdx + 1].first, botCh[botIdx + 1].second };
    Segment topseg{ topP1, topP2 };
    Segment botSeg{ botP1, botP2 };
    if (topseg.onSeg(workPoint) || botSeg.onSeg(workPoint))
        return -1;
    DirLine dr1{ topP1, topP2 };
    DirLine dr2{ botP1, botP2 };
    int dev1 = dr1.dev(workPoint);
    int dev2 = dr2.dev(workPoint);
    if (dev1 * dev2 == 1)
        return 0;
    return 1;
}

uint getMaxElemIdx(const vector<pair<double, double>>& conHull) { // max X
    double ref = DBL_MIN;
    double max = ref;
    uint k = 0;
    for (uint i = 0; i < conHull.size(); i++) {
        if (conHull[i].first >= max) {
            max = conHull[i].first;
            k = i;
        }
    }
    if (max == ref)
        exit(123);
    return k;
}

bool inputPoints(ifstream& fin)
{   //input polygon
    if (!fin) return false;
    double firstX, firstY;
    double tmp1, tmp2;
    fin >> firstX;
    fin >> firstY;
    ch.push_back(make_pair(firstX, firstY));
    fin >> tmp1;
    fin >> tmp2;
    ch.push_back(make_pair(tmp1, tmp2));
    while (true) {
        fin >> tmp1;
        fin >> tmp2;
        if (tmp1 == firstX && tmp2 == firstY)
            break;
        else
            ch.push_back(make_pair(tmp1, tmp2));
    }
    //input workPoints
    while (!fin.eof()) {
        fin >> tmp1 >> tmp2;
        workPoints.push_back(make_pair(tmp1, tmp2));
    }
    fin.close();
    return true;
}

int segIdx(const vector<pair<double, double>>& chain, double workPointX) {
    uint lowerBound = 0;
    uint upperBound = chain.size() - 1;
    uint curIdx;

    if (workPointX > chain[upperBound].first) return upperBound;
    if (workPointX < chain[lowerBound].first) return -1;
    if (chain[lowerBound].first == chain[lowerBound + 1].first
        && chain[lowerBound + 1].first == workPointX) /// a == b == c
        return lowerBound;
    if (chain[upperBound - 1].first == chain[upperBound].first
        && chain[upperBound].first == workPointX)      /// a == b == c
        return upperBound - 1;

    while (true)
    {
        curIdx = (lowerBound + upperBound) / 2;
        if (chain[curIdx].first <= workPointX && chain[curIdx + 1].first >= workPointX) {
            return curIdx;
        }
        else {
            if (chain[curIdx].first < workPointX)
                lowerBound = curIdx + 1;
            else
                upperBound = curIdx - 1;
        }
    }
}

void displayPointSitRes(int res, Point wp) {
    cout << "(" << wp.getCoordsX() << ", " << wp.getCoordsY() << ") is ";
    if (res == -1) {
        cout << "ON\n\n";
        totalOn++;
    }
    else if (res == 1) {
        cout << "IN\n\n";
        totalIn++;
    }
    else {
        cout << "OUT\n\n";
        totalOut++;
    }
}

void writePointSitFile(ofstream& fout, int res, Point wp) {
    fout << "(" << wp.getCoordsX() << ", " << wp.getCoordsY() << ") is ";
    if (res == -1) {
        fout << "ON\n\n";
        totalOn++;
    }
    else if (res == 1) {
        fout << "IN\n\n";
        totalIn++;
    }
    else {
        fout << "OUT\n\n";
        totalOut++;
    }
}