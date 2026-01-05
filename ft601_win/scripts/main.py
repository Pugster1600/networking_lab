# Python
#import ftd3xx #need to switch the driver to the non wu version

#print("hello world")
# List all connected FT3xx devices
#devices = ftd3xx.listDevices()
#print("Connected FT3xx Devices:", devices)

import sympy as smp

v1, v2, v3, v4, r1, r2, r3, r4, rf, vout = smp.symbols('v1, v2, v3, v4, r1, r2, r3, r4, rf, vout')

vout = smp.Eq(vout, -rf*((v1/r1)+(v2/r2)+(v3/r3)+(v4/r4)))
binSeq = [[0,0,0,0], [0,0,0,1], [0,0,1,0], [0,0,1,1], # or can achieve with bit banging
          [0,1,0,0], [0,0,0,1], [0,0,0,1], [0,0,0,1],
          [0,0,0,1], [0,0,0,1], [0,0,0,1], [0,0,0,1],
          [0,0,0,1], [0,0,0,1], [0,0,0,1], [0,0,0,1]]

binSeq = [f"{i:04b}" for i in range(16)]
result = {}

for binary in binSeq:
  _v1 = 5 if binary[0] == '1' else 0
  _v2 = 5 if binary[1] == '1' else 0
  _v3 = 5 if binary[2] == '1' else 0
  _v4 = 5 if binary[3] == '1' else 0
  sol = vout.subs({v1: _v1, v2: _v2, v3: _v3, v4: _v4, r1: 10000, r2: 20000, r3: 40000, r4: 80000, rf: 80000})
  result[binary] = sol.rhs.evalf()
print(result)



