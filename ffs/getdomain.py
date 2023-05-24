from Crypto.PublicKey import RSA

RSAkey=RSA.generate(1024)
print("1024 keys")
print("p: ", RSAkey.p)
print("q: ", RSAkey.q)
print("n: ", RSAkey.n)

RSAkey=RSA.generate(2048)
print("2048 keys")
print("p: ", RSAkey.p)
print("q: ", RSAkey.q)
print("n: ", RSAkey.n)

RSAkey=RSA.generate(3072)
print("3072 keys")
print("p: ", RSAkey.p)
print("q: ", RSAkey.q)
print("n: ", RSAkey.n)