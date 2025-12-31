#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit
from scipy.special import lambertw

k = 1.380649e-23
e = 1.602176634e-19
T = 25 + 273.15


# APTF1616LSEEZGKQBKC
# Red
voltages = [
    1.75, 1.8, 1.825, 1.85, 1.875, 1.9, 1.925,
]
currents = [
    4e-4, 2e-3, 3.8e-3, 6e-3, 8e-3, 10e-3, 12e-3,
]

# Green
voltages = [
    2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0,
]
currents = [
    3e-4, 0.8e-3, 1.5e-3, 2.5e-3, 3.8e-3, 6.5e-3, 10e-3,
]

# Blue
voltages = [
    2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0,
]
currents = [
    2e-4, 8e-4, 1.3e-3, 3e-3, 5e-3, 7.5e-3, 10e-3,
]


def diode(Vd, Is, n, Rs):
    Vt = k*T/e
    w = lambertw(Is*Rs/n/Vt*np.exp(Vd/n/Vt))
    Id = n*Vt/Rs*w
    return Id.real


popt = curve_fit(
    diode, voltages, currents, [50e-15, 1.1, 2.0],
    bounds=([1e-15, 0.1, 1e-15], [1.0, 20.0, 20.0]),
    maxfev=10000, full_output=True,
    xtol=1e-15
)


noff = 0.00  # -0.25
Rsoff = 0  # 6
print(popt)
print(popt[0][0], popt[0][1]+noff, popt[0][2]+Rsoff)

vlin = np.linspace(0, 3.0, 100)

plt.figure(1)
plt.plot(voltages, currents, 'rx')
plt.plot(vlin, diode(vlin, popt[0][0], popt[0][1]+noff, popt[0][2]+Rsoff), 'b')
plt.grid()
plt.show()
