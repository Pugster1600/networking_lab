import numpy as np
#import scipy.signal as signal
import matplotlib.pyplot as plt
# Parameters
fs = 500e3            # Sampling rate (500 kHz)
T = 0.01              # Total duration in seconds
Vpp = 0.01            # Peak-to-peak voltage (10 mV)
f_max = 20000         # Max frequency component (20 kHz)
noise_std = 0.001     # Standard deviation of Gaussian noise (in volts)

# Time array
t = np.arange(0, T, 1/fs)

# Generate a random signal with frequency components up to 20 kHz
N = len(t)
frequencies = np.fft.rfftfreq(N, 1/fs)
amplitudes = np.zeros(len(frequencies))

# Assign random amplitudes to frequencies up to 20 kHz
mask = frequencies <= f_max
amplitudes[mask] = np.random.rand(np.sum(mask)) - 0.5  # centered around 0

# Construct signal via IFFT
signal = np.fft.irfft(amplitudes, n=N)

# Normalize to desired peak-to-peak voltage
signal = signal / np.max(np.abs(signal)) * (Vpp / 2)

# Add Gaussian noise
noise = np.random.normal(0, noise_std, size=N)
signal_noisy = signal + noise

# Save as PWL file for LTspice
with open("signal_pwl.txt", "w") as f:
    for ti, vi in zip(t, signal_noisy):
        f.write(f"{ti:.9f} {vi:.6f}\n")

print("PWL file 'signal_pwl.txt' with Gaussian noise generated successfully!")
plt.plot(t, signal_noisy)
plt.title("Generated Signal with Gaussian Noise")
plt.xlabel("Time (s)")
plt.ylabel("Voltage (V)")
plt.grid()
plt.show()