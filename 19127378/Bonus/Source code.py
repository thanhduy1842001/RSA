import random
die = random.SystemRandom()
bits = 256
# RSA
def single_test(n, a):
    exp = n - 1
    while not exp & 1:
        exp >>= 1
        
    if fastPowerMod(a, exp, n) == 1:
        return True
        
    while exp < n - 1:
        if fastPowerMod(a, exp, n) == n - 1:
            return True
            
        exp <<= 1
        
    return False
    
def millerRabin(n,k=40):
    for i in range(k):
        a = die.randrange(2, n - 1)
        if not single_test(n, a):
            return False
            
    return True

def genPrime():
    while True:
        a = (die.randrange(1 << bits - 1, 1 << bits) << 1) + 1
        if millerRabin(a):
            return a

def extendedEuclide(a,b):
    m = a; xm = 1; ym = 0
    n = b; xn = 0; yn = 1
    while n!=0:
        q = m // n
        r = m % n
        xr = xm - q*xn
        yr = ym - q*yn
        m,xm,ym = n,xn,yn
        n,xn,yn = r,xr,yr
    if xm<0: xm += b
    return xm

def findPrivateKey(e,p,q):
    phi = (p-1)*(q-1)
    d = extendedEuclide(e,phi)
    return d

def fastPowerMod(a,k,n):
    res = 1
    while k != 0:
        if k&1==1: 
            res*=a
            res%=n
        a*=a
        a%=n
        k>>=1
    return res

def decryp(c,d,n):
    return fastPowerMod(c,d,n)

def encryp(m,e,n):
    return fastPowerMod(m,e,n)

def genKey():
    p = 3
    q = 7
    while not millerRabin(q):
        q-=1
    # Tim so nguyen to p gan q nhat de mo phong tan cong phan tich Fermat 
    n = p*q
    e = 11
    d = findPrivateKey(e,p,q)
    return e,d,n

# attackRSA

def isqrt(n):
    x = n
    y = (x + n // x) // 2
    while y < x:
        x = y
        y = (x + n // x) // 2
    return x

def fermatAttackRSA(n):
    a = isqrt(n)
    b2 = a*a - n
    b = isqrt(n)
    while b*b != b2:
        a += 1
        b2 = a*a - n
        b = isqrt(b2)
    p = a+b
    q = a-b
    return p,q

def attackRSA(c,n,e):
    p, q = fermatAttackRSA(n)
    d = findPrivateKey(e,q,p)
    m = encryp(c,d,n)
    return p,q,d,m

print("Cho m = 19127378")
m=5
# Test RSA
print("Ma hoa m bang RSA: ")
e,d,n = genKey()
print(f'e = {e}')
print(f'd = {d}')
print(f'n = {n}')
c = encryp(m,e,n)
m = encryp(c,d,n)
print (f'c = {c,m}')

# Test attackRSA
print("Tan cong RSA:")
p,q,d,m = attackRSA(c,n,e)
print(f'p = {p}')
print(f'q = {q}')
print(f'd = {d}')
print(f'm = {m}')

