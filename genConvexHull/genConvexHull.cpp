#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <fstream>
#include <string>
using namespace std;
typedef unsigned int uint;

const uint N = 100;
uint n = 0;
void genRandPoints(); // don't use
inline double f(double x);
void fileWriter(const vector<pair<double, double>>& vect, string fname, uint elemCount);
void outputData(string filename, const vector<pair<double, double>>& topCH,
	const vector<pair<double, double>>& botCH, const vector<pair<double, double>>& workPoints);
vector<pair<double, double>> genWorkPoints(uint n_);

int main()
{
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist1(0, (double)N/10);
	uniform_real_distribution<double> dist2(0, (double)N/100);
	auto random1 = bind(dist1, mt);
	auto random2 = bind(dist2, mt);
	vector<pair<double, double>> topCH;
	vector<pair<double, double>> botCH;
	double x = random1();
	double x1 = x;
	topCH.push_back(make_pair(x, f(x)));
	botCH.push_back(make_pair(x1, -f(x1)));
	n++;
	while (x < (double)N && x1 < (double)N - 2.0) {
		x = topCH[n - 1].first + random2();
		x1 = topCH[n - 1].first + random2();
		topCH.push_back(make_pair(x, f(x)));
		botCH.push_back(make_pair(x1, -f(x1)));
		n++;
	}
	auto workPoints = genWorkPoints(n/4);

	fileWriter(topCH, "upperCH_"+to_string(N) +".csv", n);
	fileWriter(botCH, "bottomCH_" + to_string(N) + ".csv", n);
	fileWriter(workPoints, "workPoints_" + to_string(N) + ".csv", workPoints.size());
	//reverse(botCH.begin(), botCH.end());
	ofstream fout3("completeCH.csv", ios::out);
	fout3 << "x, y\n";
	for (uint i = 0; i < topCH.size(); i++)
		fout3 << topCH[i].first << ", " << topCH[i].second << "\n";
	for (uint i = 0; i < botCH.size(); i++)
		fout3 << botCH[n - i - 1].first << ", " << botCH[n - i - 1].second << "\n";
	fout3.close();
	outputData("input" + to_string(N) + ".txt", topCH, botCH, workPoints);
	return 0;
}

void outputData(string filename, const vector<pair<double, double>>& topCH,
	const vector<pair<double, double>>& botCH, const vector<pair<double, double>>& workPoints)
{
	ofstream fout(filename, ios::out);
	for (uint i = 0; i < topCH.size(); i++)
		fout << topCH[i].first << " " << topCH[i].second << "\n";
	for (uint i = 0; i < botCH.size(); i++)
		fout << botCH[n - i - 1].first << ", " << botCH[n - i - 1].second << "\n";
	fout << topCH[0].first << " " << topCH[0].second <<"\n\n";

	for (uint i = 0; i < workPoints.size(); i++)
		fout << workPoints[i].first << " " << workPoints[i].second << "\n";
	fout.close();
	return;
}

vector<pair<double, double>> genWorkPoints(uint n_) {
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist1(0, (double)N);
	auto random1 = bind(dist1, mt);
	vector<pair<double, double>> res;
	for (uint i = 0; i < n_; i++)
		res.push_back(make_pair(random1(), random1() - random1()));
	return res;
}

void genRandPoints()
{
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist1(0, 10);
	uniform_real_distribution<double> dist2(-10, 0);
	auto random1 = bind(dist1, mt);
	auto random2 = bind(dist2, mt);
	vector<double> xarrPos;
	vector<double> xarrNeg;
	vector<double> yarrPos;
	vector<double> yarrNeg;
	vector<pair<double, double>> topLeft;
	vector<pair<double, double>> topRight;
	vector<pair<double, double>> bottomLeft;
	vector<pair<double, double>> bottomRight;
	xarrPos.push_back(round(random1()));
	xarrNeg.push_back(round(random2()));
	yarrPos.push_back(round(random1()));
	yarrNeg.push_back(round(random2()));
	for (uint i = 1; i < N; i++) {
		xarrPos.push_back(xarrPos[i - 1] + round(random1()));
		xarrNeg.push_back(xarrNeg[i - 1] - round(random2()));
		yarrPos.push_back(yarrPos[i - 1] + round(random1()));
		yarrNeg.push_back(yarrNeg[i - 1] - round(random2()));
	}
	sort(xarrPos.begin(), xarrPos.end(), less<>());
	sort(xarrNeg.begin(), xarrNeg.end(), less<>());
	sort(yarrPos.begin(), yarrPos.end(), less<>());
	sort(yarrNeg.begin(), yarrNeg.end(), less<>());

	for (uint i = 0; i < N; i++) {
		topLeft.push_back(make_pair(xarrNeg[i], yarrPos[i]));
		topRight.push_back(make_pair(xarrPos[i], yarrPos[N - i - 1]));
		bottomLeft.push_back(make_pair(xarrNeg[i], yarrNeg[N - i - 1]));
		bottomRight.push_back(make_pair(xarrPos[i], yarrPos[i]));
	}

	ofstream fout("upperCH.csv", ios::out);
	fout << "x, y\n";
	for (uint i = 0; i < N; i++)
		fout << topLeft[i].first << ", " << topLeft[i].second << "\n";
	for (uint i = 0; i < N; i++)
		fout << topRight[i].first << ", " << topRight[i].second << "\n";
	fout.close();

	ofstream fout2("bottomCH.csv", ios::out);
	fout2 << "x, y\n";
	for (uint i = 0; i < N; i++)
		fout2 << bottomLeft[i].first << ", " << bottomLeft[i].second << "\n";
	for (uint i = 0; i < N; i++)
		fout2 << bottomRight[i].first << ", " << bottomRight[i].second << "\n";
	fout2.close();

	ofstream fout3("checkPoints.csv", ios::out);
	fout << "x, y\n";
	for (uint i = 0; i < N; i++) {
		double x = round((random1() + random2()));
		double y = round((random1() + random2()));
		fout3 << x << ", " << y << "\n";
	}
	fout3.close();

	ofstream fout4("completeCH.csv", ios::out);
	fout4 << "x, y\n";
	for (uint i = 0; i < N; i++)
		fout4 << topLeft[i].first << ", " << topLeft[i].second << "\n";
	for (uint i = 0; i < N; i++)
		fout4 << topRight[i].first << ", " << topRight[i].second << "\n";
	for (uint i = 0; i < N; i++)
		fout4 << bottomRight[i].first << ", " << bottomRight[i].second << "\n";
	for (uint i = 0; i < N; i++)
		fout4 << bottomLeft[i].first << ", " << bottomLeft[i].second << "\n";
	fout4.close();
}

inline double f(double x)
{
	return -0.05 * x * (x - N);
}

void fileWriter(const vector<pair<double, double>>& vect, string fname, uint elemCount)
{
	ofstream fout(fname, ios::out);
	fout << "x, y\n";
	for (uint i = 0; i < elemCount; i++)
		fout << vect[i].first << ", " << vect[i].second << "\n";
	fout.close();
}


