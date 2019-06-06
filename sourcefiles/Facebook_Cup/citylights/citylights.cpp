// City Lights
//
 
#define DEBUG 0
 
#include <algorithm>
#include <functional>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <sstream>
using namespace std;
 
#define LL long long
#define LD long double
#define PR pair<int,int>
 
#define Fox(i,n) for (i=0; i<n; i++)
#define Fox1(i,n) for (i=1; i<=n; i++)
#define FoxI(i,a,b) for (i=a; i<=b; i++)
#define FoxR(i,n) for (i=(n)-1; i>=0; i--)
#define FoxR1(i,n) for (i=n; i>0; i--)
#define FoxRI(i,a,b) for (i=b; i>=a; i--)
#define Foxen(i,s) for (i=s.begin(); i!=s.end(); i++)
#define Min(a,b) a=min(a,b)
#define Max(a,b) a=max(a,b)
#define Sz(s) int((s).size())
#define All(s) (s).begin(),(s).end()
#define Fill(s,v) memset(s,v,sizeof(s))
#define pb push_back
#define mp make_pair
#define x first
#define y second
 
template<typename T> T Abs(T x) { return(x<0 ? -x : x); }
template<typename T> T Sqr(T x) { return(x*x); }
string plural(string s) { return(Sz(s) && s[Sz(s)-1]=='x' ? s+"en" : s+"s"); }
 
const int INF = (int)1e9;
const LD EPS = 1e-12;
const LD PI = acos(-1.0);
 
#if DEBUG
#define GETCHAR getchar
#else
#define GETCHAR getchar_unlocked
#endif
 
bool Read(int &x)
{
    char c,r=0,n=0;
    x=0;
        for(;;)
        {
            c=GETCHAR();
                if ((c<0) && (!r))
                    return(0);
                if ((c=='-') && (!r))
                    n=1;
                else
                if ((c>='0') && (c<='9'))
                    x=x*10+c-'0',r=1;
                else
                if (r)
                    break;
        }
        if (n)
            x=-x;
    return(1);
}
 
#define LIM 135
#define MOD 1000000007
 
int W,S,K,NH;
PR PW[LIM],PS[LIM];
int H[LIM];
vector<int> ch[LIM],wh[LIM];
LL dyn[LIM][LIM][LIM]; // dyn[i][h][b] = # of combinations in i's subtree requiring b buildings, with height h still required
LL dynS[LIM][LIM][LIM]; // dynS[i][h][b] = sum of dyn[i][0..(h-1)][b]
LL tmp[LIM][LIM];
 
void CompSums(int i)
{
    int h,b;
        Fox(h,NH)
            Fox(b,W+1)
                dynS[i][h+1][b]=(dynS[i][h][b]+dyn[i][h][b])%MOD;
}
 
void Add(LL &a,LL b)
{
    a=(a+b)%MOD;
}
 
void rec(int i)
{
    int j,c,h,b,h2,b2,nw;
    LL pw;
    // iterate over children
    dyn[i][0][0]=1;
        Fox(j,Sz(ch[i]))
        {
            rec(c=ch[i][j]);
            // combine DP values
            CompSums(i),CompSums(c);
            Fill(tmp,0);
                Fox(h,NH)
                    Fox(b,W+1)
                        Fox(b2,W-b+1)
                            Add(tmp[h][b+b2],dyn[i][h][b]*dynS[c][h+1][b2] + dynS[i][h][b]*dyn[c][h][b2]);
            memcpy(dyn[i],tmp,sizeof(tmp));
        }
    // consider all possible subsets of windows coming into play here
    nw=Sz(wh[i]);
    sort(All(wh[i]));
    reverse(All(wh[i]));
    Fill(tmp,0);
        Fox(j,nw+1)
        {
            h2=(j==nw ? 0 : wh[i][j]);
            pw=(1LL<<(nw-min(nw,j+1)))%MOD;
                Fox(h,NH)
                    Fox(b,W+1)
                        Add(tmp[max(h,h2)][b],pw*dyn[i][h][b]);
        }
    memcpy(dyn[i],tmp,sizeof(tmp));
    // place a building here when necessary
        FoxI(h,H[i],NH-1)
            Fox(b,W+1)
            {
                Add(dyn[i][0][b+1],dyn[i][h][b]);
                dyn[i][h][b]=0;
            }
    CompSums(i);
}
 
int main()
{
        if (DEBUG)
            freopen("in.txt","r",stdin);
    // vars
    int T,t;
    int i,x,y,z,a,b,c;
    LL ans;
    vector<int> cy;
    set<pair<PR,int> > SI;
    set<pair<PR,int> >::iterator I;
    map<int,int> MS;
    // testcase loop
    Read(T);
        Fox1(t,T)
        {
            // input, and compress y-coordinates
            Read(W),Read(S);
            cy.clear(),cy.pb(0);
                Fox(i,W)
                    Read(PW[i].y),Read(PW[i].x),cy.pb(PW[i].x);
                Fox(i,S)
                    Read(PS[i].y),Read(PS[i].x),cy.pb(PS[i].x);
            sort(All(cy));
            cy.resize(unique(All(cy))-cy.begin());
            NH=Sz(cy);
                Fox(i,W)
                    PW[i].x=lower_bound(All(cy),PW[i].x)-cy.begin();
                Fox(i,S)
                    PS[i].x=lower_bound(All(cy),PS[i].x)-cy.begin();
            // init tree
            Fill(H,60);
                Fox(i,K)
                    ch[i].clear(),wh[i].clear();
            K=0;
            // iterate over stars in non-decreasing order of Y
            sort(PS,PS+S);
            SI.clear();
            H[0]=1,SI.insert(mp(mp(0,INF+1),K++));
            MS.clear();
                Fox(i,S)
                {
                    x=PS[i].y;
                    y=PS[i].x;
                    // find containing interval (if any)
                    I=SI.lower_bound(mp(mp(x,INF+2),0)),I--;
                    a=I->x.x;
                    b=I->x.y;
                    c=I->y;
                        if ((a<=x) && (b>=x))
                        {
                            // split interval at this X-coordinate
                                if (a<x)
                                {
                                    ch[c].pb(K);
                                    H[K]=y,SI.insert(mp(mp(a,x-1),K++));
                                }
                                if (b>x)
                                {
                                    ch[c].pb(K);
                                    H[K]=y,SI.insert(mp(mp(x+1,b),K++));
                                }
                            SI.erase(I);
                            MS[x]=c;
                        }
                }
            // associate windows with nodes
                Fox(i,W)
                {
                    x=PW[i].y;
                        if (MS.count(x))
                            c=MS[x];
                        else
                        {
                            I=SI.lower_bound(mp(mp(x,INF+2),0)),I--;
                            c=I->y;
                        }
                    wh[c].pb(PW[i].x);
                }
            // DP
            Fill(dyn,0);
            rec(0);
            ans=0;
                Fox1(i,W)
                    Add(ans,i*dyn[0][0][i]);
            // output
            printf("Case #%d: %lld\n",t,ans);
        }
    return(0);
}
