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

//GyS loves algorithm
#define x first
#define y second
#define all(x) x.begin(), x.end()
#define mk(x, y) make_pair(x, y)
#define pb(x) push_back(x)
#define rep(x, n) for (int x = 0; x < n; x++)
#define range(x, a, b)for (int x = a; x <= b; x++)
#define sz(x) x.size()
#define setv(x, y) memset(x, y, sizeof(x))
#define repi(it, x) for (typeof(x.begin()) it = x.begin(); it != x.end(); ++it)
#define pl()  printf("\n")
#define pok() printf("line %d ok\n", __line__);
//for segment tree
#define lc rt<<1, l, mid
#define rc rt<<1|1, mid + 1, r

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef	vector<int> vti;

const int inf = 1 << 30;

pii operator +(const pii& a, const pii& b) { return pii(a.first+b.first, a.second+b.second); }
pii operator -(const pii& a, const pii& b) { return pii(a.first-b.first, a.second-b.second); }
ll operator *(const pii& a, const pii& b) { return (ll)a.first*b.first + (ll)a.second*b.second; }
ll operator %(const pii& a, const pii& b) { return (ll)a.first*b.second - (ll)a.second*b.first; }

template<typename type> struct point { type x, y; point(type a = 0, type b = 0): x(a), y(b) { } };
template<typename type> type sqr(type x) {return x * x; }
template<typename t> void upto(t& a, const t& b) { if (a < b) a = b; }
template<typename t> void dnto(t& a, const t& b) { if (a > b) a = b; }

const char *dirbase = "/home/jianchuan.gys/weather/";
char line[110];

int month[1700];
int days[30] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
bool leap[3000];


void gao(const char *city)
{
	double ans = -1;
	int day;
	for (int i = 0; i < 1700; i++) {
		int t = i + 199601;
		int y = t / 100;
		int m = t % 100;
		int d = days[m];
		if (leap[y] && m == 2)
			d++;
		double ret = (double)month[i] / d;
		if (ans < ret) {
			ans = ret;
			day = t;
		}
	}
	printf("%15s %6d %f\n", city, day, ans);
}

int main()
{
	freopen("input.txt", "r", stdin);

	memset(leap, false, sizeof(leap));
	for (int i = 1996; i <= 2020; i++) {
		if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0)) {
			leap[i] = true;
		}
	}


	memset(month, 0, sizeof(month));
	char lastcity[110] = "nocity";
	char city[110];
	bool bb = true;
	int tt;
	int max_t = -inf, min_t = -inf; 
	while (gets(line)) {
		int cnt = 0;
		max_t = -inf;
		min_t = -inf;
		for (int i = 0; line[i]; ) {
			if (line[i] == 'w') {
				while (line[i] != '/')
					i++;
				i++;
				if (line[i]) {
					int cc = 0;
					while (line[i] != '/') {
						city[cc++] = line[i];
						i++;
					}
					i++;
					city[cc] = '\0';
				}
				if (line[i]) {
					tt = 0;
					while (line[i] != '-') {
						tt = tt * 10 + line[i] - '0';
						i++;
					}
					i++;
					while (line[i] != '-') {
						tt = tt * 10 + line[i] - '0';
						i++;
					}
					i++;
					tt -= 199601;
				}
			} else if (line[i] == ':') {
				i++;
				int sign = 1;

				if (line[i] == '-') {
					sign = -1;
					i++;
				}

				int tmp = 0;
				while (line[i] >= '0' && line[i] <= '9') {
					tmp = tmp * 10 + line[i] - '0';
					i++;
				}
				tmp = sign * tmp;
				if (max_t == -inf) {
					max_t = tmp;

				} else {
					min_t = tmp;
					if (strcmp(lastcity, city) == 0) {
						month[tt] += max_t - min_t;
					} else {
						if (bb) {
							bb = false;
						} else {

							gao(lastcity);
						}

						strcpy(lastcity, city);
						memset(month, 0, sizeof(month));
						month[tt] += max_t - min_t;
					}
				}
			} else {
				i++;
			}
		}
	}
	gao(lastcity);

	return 0;
}
				
