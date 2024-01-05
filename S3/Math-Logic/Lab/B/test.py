# Just to show how I was tesing my solution on haskell by using correct c++ solution.
# It worked - haskell solution is now correct.

import random as rnd
import subprocess as subp

VARS = ['A', 'B', 'C']
OPS = ['&', '|', '->']

def make(level, was_negate = False):
    if level == 0:
        return rnd.choice(VARS)
    
    if was_negate or rnd.randint(0, 100) < 50:
        l = make(rnd.randint(0, level - 1))
        r = make(rnd.randint(0, level - 1))

        op = rnd.choice(OPS)

        return f'{l}{op}{r}'
    else:
        t = make(rnd.randint(0, level - 1), True)

        return f'!({t})' if len(t) > 1 else f'!{t}'

def gen():
    result = ''
    for i in range(1, rnd.randint(5, 15)):
        result += "|-" + make(rnd.randint(10, 15)) + '\n'

    return result

def seed(cmd, data):    
    p = subp.Popen(cmd, stdin=subp.PIPE, stdout=subp.PIPE)
    p.stdin.write(data.encode())
    p.stdin.close()
    p.wait()
    return p.stdout.read().decode()

def test(amount):
    for i in range(amount):
        data = gen()

        cpp = seed([r'.\B.exe'], data).strip().replace("\r\n", "\n")
        hs = seed(['wsl', '/home/shady/src/logic/b/main'], data).strip()

        if cpp != hs:
            print(data)

test(100000)