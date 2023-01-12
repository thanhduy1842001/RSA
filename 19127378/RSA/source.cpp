// OS: Windown 10
// IDE: VS Code 
// Language: C++14
#include <iostream>
#include <bitset>
#include <vector>
#include <time.h>
#include <random>
#include <time.h>
using namespace std;
#define size 256
bitset<size> randNum(int bits)
{
    bitset<size> res;
    res[0]=1;
    res[bits - 1]=1;
    for (int i = 2; i < bits - 1; i++) res[i]=rand()&1;
    return res;
}
bitset<size> Neg(bitset<size> a)
{
    int i = 0;
    while(i<size and !a[i]) i++;
    while(i<size-1)
    {
        i++;
        a[i]=!a[i];
    }
    return a;
}
bitset<size> hexToBin(string a)
{
    bitset<size> res;
    bool sign=0;
    int n = 0, tmp;
    while(a[0]=='-') 
    {
        sign=!sign;
        a.erase(0,1);
    }
    for (int i = a.length()-1; i>=0;i--)
    {
        if (a[i]>='0' and a[i]<='9') tmp = int(a[i]) - '0';
        else if (a[i]>='A' and a[i]<='F') tmp = int(a[i]) - 'A' + 10;
        for (int i = 3; i>=0; i--) res[n++] = (tmp>>(3-i))&1;
    }
    return sign ? Neg(res) : res;
}
string binToHex(bitset<size> a)
{
    string res="";
    int tmp,cnt=size-1;
    if(a[size-1]) 
    {
        res+='-';
        a = Neg(a);
    }
    while(!a[cnt]) cnt--;
    while(cnt%4!=3) cnt++;
    for (int i=cnt;i>=0;i-=4)
    {
        tmp = 0;
        for(int j=3;j>=0;j--) tmp=tmp|(a[i-j]<<(3-j));
        if (tmp>=0 and tmp <= 9) res+= char('0'+tmp);
        else res += char('A'+ tmp-10);
    }
    if(res=="") return "0";
    return res;
}
bitset<size> sumBigInt(bitset<size> a, bitset<size> b)
{
    bitset<size> res(size);
    int c=0,tmp;
    for (int i = 0; i<size; i++)
    {
        tmp = c + a[i] + b[i];
        if(tmp == 0)
        {
            res[i]=0;
            c = 0;
        }
        else if(tmp ==1)
        {
            res[i] = 1;
            c = 0;
        }
        else if(tmp ==2)
        {
            res[i] = 0;
            c = 1;
        }
        else if(tmp ==3)
        {
            res[i] = 1;
            c = 1;
        }
    }
    return res;
}
bool checkGreater(bitset<size> a, bitset<size> b)
{
    int i=size-1;
    while(i>=0 and a[i]==b[i]) i--;
    if(i<0) return 1;
    return a[i]>b[i];
}
bitset<size> mulBigInt(bitset<size> A, bitset<size> B)
{
    bitset<size> res;
    int cnt = size - 1;
    while(!B[cnt]) cnt--;
    for(int i=0;i<=cnt;i++)
    {
        if(B[i]) res=sumBigInt(res,A);
        A<<=1;
    }
    return res;
}
bitset<size> divBigInt(bitset<size> A, bitset<size> B, bitset<size> &r)
{
    int cnt1 = size-1, cnt2 = size-1,cnt;
    bitset<size> res;
    while(!A[cnt1]) cnt1--;
    while(!B[cnt2]) cnt2--;
    cnt = cnt1 - cnt2;
    B<<=cnt;
    while(1)
    {
        while(cnt1>-1 && !A[cnt1]) 
        {
            cnt1--;
            cnt--;
            B>>=1;
        }
        if(!checkGreater(A,B))
        {
            cnt1--;
            cnt--;
            B>>=1;
        }
        if(cnt1<cnt2) break;
        res[cnt] = 1;
        A=sumBigInt(A,Neg(B));
    }
    r = A;
    return res;
}
bitset<size> modBigInt(bitset<size> A, bitset<size> B)
{
    int cnt1 = size -1, cnt2 = size -1;
    while(!A[cnt1]) cnt1--;
    while(!B[cnt2]) cnt2--;
    if(cnt2>cnt1) goto end;
    B<<=(cnt1 - cnt2);
    while(1)
    {
        while(!A[cnt1]) 
        {
            cnt1--;
            B>>=1;
        }
        if(!checkGreater(A,B))
        {
            cnt1--;
            B>>=1;
        }
        if(cnt1<cnt2) break;
        A=sumBigInt(A,Neg(B));
    }
end:
    return A;
}
bitset<size> fastPowerMod(bitset<size> a, bitset<size> k, bitset<size> n)
{
    int cnt = size - 1;
    bitset<size> res,tmp1,tmp2;
    res[0]=1;
    while(!k[cnt]) cnt--;
    for(int i=0; i<=cnt;i++)
    {
        if(k[i]) 
        {
            res = mulBigInt(a,res);
            res = modBigInt(res,n);
        }
        a = mulBigInt(a,a);
        a = modBigInt(a,n);
    }
    return res;
}
bool equal(bitset<size> a, bitset<size> b)
{
    int cnt = 0;
    while(cnt<size && a[cnt] == b[cnt]) cnt++;
    return (cnt==size);
}
bool checkZero(bitset<size> a)
{
    int cnt = 0;
    while(cnt<size && !a[cnt]) cnt++;
    return (cnt==size);
}
bool checkOne(bitset<size> b)
{
    if(b[0]==0) return 0;
    b[0]=0;
    return checkZero(b);
}
bool millerRabinTest(bitset<size> a, bitset<size> p)
{
    bitset<size> m = p, b,tmp;
    int s = 0;
    m[0]=0;
    tmp = m;
    while(!m[0]) 
    {
        m>>=1;
        s++;
    }
    b = fastPowerMod(a,m,p);
    if(checkOne(b) or equal(b,tmp)) return true;
    for(int i = 0; i < s; i++)
    {
        m<<=1;
        b = fastPowerMod(a,m,p);
        if(equal(b,tmp)) return true;
    }
    return false;
}
bool millerRabin(bitset<size> p)
{
    vector<int> smallPrime = {2,3,5,7,11};

    for(auto i : smallPrime)
    {
        bitset<size> a(i);
        if (!millerRabinTest(a,p)) return false;
    }
    return true;
}
bitset<size> genPrime(int bits)
{
    bitset<size> p;
    cout<<"Sinh ngau nhien so nguyen to "<<bits<<" bits: ";
    while(1)
    {
        p = randNum(bits);
        cout<<".";
        if(millerRabin(p)) 
        {
            cout<<endl;
            return p;
        }
    }
}
bool extendedEuclide(bitset<size> a, bitset<size> n, bitset<size> &d)
{
    bitset<size> r, q, x=n, y=a, X, Y, R;
    Y[0]=1;
    while (!checkZero(y))
    {
        q = divBigInt(x,y,r);
        R = sumBigInt(X,Neg(mulBigInt(q,Y)));
        x=y;
        X=Y;
        y=r;
        Y=R;
    }
    if(checkOne(x))
    {
        while(X[size-1]) X=sumBigInt(X,n);
        d = X;
        return true;
    }
    return false;
}
bitset<size> genKey(bitset<size> &e, bitset<size> &d, bitset<size> &n)
{
    int bits = 64;
    bitset<size> one;
    one[0]=1;
    one = Neg(one);
    bitset<size> p = genPrime(bits);
    cout<<"p: "<<binToHex(p)<<endl;
    bitset<size> q = genPrime(bits);
    cout<<"q: "<<binToHex(q)<<endl;
    n = mulBigInt(p,q);
    p = sumBigInt(p,one);
    q = sumBigInt(q,one);
    bitset<size> phi = mulBigInt(p,q);
    do
    {
    e = randNum(bits);
    }while (!extendedEuclide(e,phi,d));
}
bitset<size> Encrypt(bitset<size> m, bitset<size> e, bitset<size> n)
{
    return fastPowerMod(m,e,n);
}
bitset<size> Decrypt(bitset<size> c, bitset<size> e, bitset<size> n)
{
    return fastPowerMod(c,e,n);
}
int main()
{
    srand(time(NULL));
    string s;
    cout<<"Nhap m(Hex): ";
    cin>>s;
    bitset<size> m = hexToBin(s);
    bitset<size> e,d,n,c;
    genKey(e,d,n);
    cout<<"e: "<<binToHex(e)<<endl;
    cout<<"d: "<<binToHex(d)<<endl;
    cout<<"n: "<<binToHex(n)<<endl;
    c = Encrypt(m,e,n);
    cout<<"c: "<<binToHex(c)<<endl;

    // thu giai ma lai de kiem tra
    m = Decrypt(c,d,n);
    cout<<"m: "<<binToHex(m)<<endl;
    system("Pause");
}