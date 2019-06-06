// The Claw
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
 
#define LIM 2100000
 
PR P[LIM],R[LIM],S[LIM];
vector<int> YP[LIM];
vector<PR> YR[LIM];
int sz;
int tree[LIM],lazy[LIM];
 
void Prop(int i)
{
    tree[i]+=lazy[i];
        if (i<sz)
        {
            lazy[i<<1]+=lazy[i];
            lazy[(i<<1)+1]+=lazy[i];
        }
    lazy[i]=0;
}
 
void Update(int i,int r1,int r2,int a,int b,int v)
{
    Prop(i);
        if ((a<=r1) && (r2<=b))
        {
            lazy[i]+=v;
            Prop(i);
            return;
        }
    int c=i<<1,m=(r1+r2)>>1;
        if (a<=m)
            Update(c,r1,m,a,b,v);
        if (b>m)
            Update(c+1,m+1,r2,a,b,v);
    Prop(c),Prop(c+1);
    tree[i]=max(tree[c],tree[c+1]);
}
 
int main()
{
        if (DEBUG)
            freopen("in.txt","r",stdin);
    // vars
    int T,t;
    int N,M,K;
    int i,j,k,s,y,a,b,p,d;
    LL ans;
    // testcase loop
    Read(T);
        Fox1(t,T)
        {
            // init
                Fox(i,LIM)
                    YP[i].clear(),YR[i].clear();
            // input
            Read(N),Read(M);
            ans=M;
                Fox(i,N)
                {
                    Read(P[i].x),Read(P[i].y);
                    ans-=P[i].y;
                    YP[P[i].y].pb(P[i].x);
                        if (i)
                            R[i-1]=mp(max(P[i-1].x,P[i].x),min(P[i-1].x,P[i].x));
                }
            // associate intervals with their max contained heights
            sort(P,P+N);
            sort(R,R+N-1);
            j=s=0;
                Fox(i,N-1)
                {
                    a=R[i].y,b=R[i].x;
                    // maintain convex hull of max heights so far
                        while ((j<N) && (P[j].x<=b))
                        {
                                while ((s) && (S[s-1].y<=P[j].y))
                                    s--;
                            S[s++]=P[j++];
                        }
                    // binary search for max height no earlier than the start of this interval
                    k=lower_bound(S,S+s,mp(a,0))-S;
                    ans+=(y=S[k].y)+1;
                    YR[y].pb(R[i]);
                }
            // process set of targets at each height
                Fox(i,LIM)
                    if (K=Sz(YP[i]))
                    {
                        // sort targets/intervals at this height
                        sort(All(YP[i]));
                        sort(All(YR[i]));
                        // segment-tree-assisted DP
                            for(sz=1;sz<K+1;sz<<=1);
                        memset(tree,0,sizeof(int)*sz*2);
                        memset(lazy,0,sizeof(int)*sz*2);
                        k=0,d=-1;
                            Fox(j,K+1)
                            {
                                // update prior DP values for intervals ending at this target
                                p=j==K ? INF : YP[i][j];
                                    while ((k<Sz(YR[i])) && (YR[i][k].x<p))
                                    {
                                        a=lower_bound(All(YP[i]),YR[i][k++].y)-YP[i].begin();
                                        Update(1,0,sz-1,0,a,1);
                                    }
                                // compute DP value
                                d=max(d+1,tree[1]);
                                Update(1,0,sz-1,j,j,d);
                            }
                        ans-=d;
                    }
            // output
            printf("Case #%d: %lld\n",t,ans*2);
        }
    return(0);
}
