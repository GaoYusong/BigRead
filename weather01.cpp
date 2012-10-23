#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <queue>
#include <set>
#include <stack>
#include <list>
#include <dirent.h>

using namespace std;

//GyS Loves Algorithm
#define X first
#define Y second
#define all(x) x.begin(), x.end()
#define mk(x, y) make_pair(x, y)
#define pb(x) push_back(x)
#define rep(x, n) for (int x = 0; x < n; x++)
#define range(x, a, b)for (int x = a; x <= b; x++)
#define sz(x) x.size()
#define setv(x, y) memset(x, y, sizeof(x))
#define repi(it, x) for (typeof(x.begin()) it = x.begin(); it != x.end(); ++it)
#define pl()  printf("\n")
#define pok() printf("Line %d ok\n", __LINE__);
//for segment tree
#define lc rt<<1, l, mid
#define rc rt<<1|1, mid + 1, r

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef	vector<int> vti;

pii operator +(const pii& a, const pii& b) { return pii(a.first+b.first, a.second+b.second); }
pii operator -(const pii& a, const pii& b) { return pii(a.first-b.first, a.second-b.second); }
ll operator *(const pii& a, const pii& b) { return (ll)a.first*b.first + (ll)a.second*b.second; }
ll operator %(const pii& a, const pii& b) { return (ll)a.first*b.second - (ll)a.second*b.first; }

template<typename Type> struct Point { Type x, y; Point(Type a = 0, Type b = 0): x(a), y(b) { } };
template<typename Type> Type sqr(Type x) {return x * x; }
template<typename T> void upto(T& a, const T& b) { if (a < b) a = b; }
template<typename T> void dnto(T& a, const T& b) { if (a > b) a = b; }

const char *dirbase = "/home/jianchuan.gys/weather/";

int month[1700]; 
int cnt[1700];

int days[10][10][10][10][10][10];
int BB = 199601;
int d[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

inline
int get(int x)
{
	int y = x / 100,
		m = x % 100,
		da = d[m];

	if (m == 2 && (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0))) {
		da++;
	}
	return da;
}

void init()
{
	rep(i, 1700) {
		int x = i + BB;
		days[x / 100000 % 10]
			[x / 10000 	% 10]
			[x / 1000 	% 10]
			[x / 100 	% 10]
			[x / 10 	% 10]
			[x / 1 		% 10] = 

			i;
		cnt[i] = get(x);
	}
}

double gao(char *cityname, char *citydir, int &da)
{
	// printf("%s %s\n", cityname, citydir);
	DIR *dir = opendir(citydir);
	struct dirent *dnt;
	char filepath[256];
	char buf[1024];

	// pok();

	setv(month, 0);
	while ((dnt = readdir(dir)) != NULL) {
		if (dnt->d_name[0] >= '0' && dnt->d_name[0] <= '9') {
			strcpy(filepath, citydir);
			strcat(filepath, "/");
			strcat(filepath, dnt->d_name);
			FILE *fp = fopen(filepath, "r");

			// pok();
			// printf("%s\n", dnt->d_name);
			int Index = days[dnt->d_name[0] - '0'][dnt->d_name[1] - '0'][dnt->d_name[2] - '0'][dnt->d_name[3] - '0']
			[dnt->d_name[5] - '0'][dnt->d_name[6] - '0'];

			// pok();

			fgets(buf, 1000, fp);

			// printf("%s\n", buf);

			int cc = 0;
			int t[2] = {0, 0};
			for (int i = 0; buf[i]; ) {
				if (buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9')) {
					int sign = 1;
					if(buf[i] == '-') {
						sign = -1;
						i++;
					}

					while (buf[i] >= '0' && buf[i] <= '9') {
						t[cc] = t[cc] * 10 + buf[i] - '0';
						i++;
					}
					t[cc] = sign * t[cc];
					cc++;
				} else {
					i++;
				}
			}

			// pok();

			month[Index] += t[0] - t[1];

			// printf("%s %s %s %d %d %d %d %d\n", cityname, dnt->d_name,  buf, t[0], t[1], Index, cnt[Index], month[Index]);
			fclose(fp);
		}
	}

	double x = -1;
	rep(i, 1700) {
		double ret = (double)month[i] / cnt[i];
		if (x < ret) {
			x = ret;
			da = i + BB;
		}
	}
	return x;
}


int main()
{

	init();

	DIR *dir = opendir(dirbase);

	// pok();

	char citydir[256];
	char citys[1100][50];
	int citys_cnt = 0;
	double ans[1100];
	int ans2[1100];
	struct dirent *dnt;
	while ((dnt = readdir(dir)) != NULL) {
		strcpy(citydir, dirbase);
		strcat(citydir, dnt->d_name);
		// pok();
		if (dnt->d_name[0] >= 'A' && dnt->d_name[0] <= 'Z') {
			double ret = gao(dnt->d_name, citydir, ans2[citys_cnt]);
			strcpy(citys[citys_cnt], dnt->d_name);
			ans[citys_cnt] = ret;
			citys_cnt++;
		}
		// pok();
	}
	rep(i, citys_cnt) {
		printf("%15s %6d %f\n", citys[i], ans2[i], ans[i]);
	}
	return 0;
}
				
