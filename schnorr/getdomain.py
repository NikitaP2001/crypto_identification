from Crypto.PublicKey import DSA
DSAkey=DSA.generate(1024)
print("1024 keys")
print("p: ", DSAkey.p)
print("q: ", DSAkey.q)
print("g: ", DSAkey.g)

DSAkey=DSA.generate(2048)
print("2048 keys")
print("p: ", DSAkey.p)
print("q: ", DSAkey.q)
print("g: ", DSAkey.g)

DSAkey=DSA.generate(3072)
print("p: ", DSAkey.p)
print("q: ", DSAkey.q)
print("g: ", DSAkey.g)