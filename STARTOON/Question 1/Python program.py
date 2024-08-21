import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import find_peaks

def load_data(filepath):
    
    return np.loadtxt(filepath)

def find_peaks_and_minima(data, distance=20, prominence=0.5):
    """Find maxima and minima in the data."""
    peaks, _ = find_peaks(data, distance=distance, prominence=prominence)
    minima, _ = find_peaks(-data, distance=distance, prominence=prominence)
    return peaks, minima

def plot_signal_with_peaks(data, peaks, minima, label):
    """Plot the signal along with its maxima and minima."""
    plt.figure(figsize=(10, 6))
    plt.plot(data, label=f'{label} Signal', color='black')
    plt.plot(peaks, data[peaks], 'ro', label='Maxima')
    plt.plot(minima, data[minima], 'bo', label='Minima')
    plt.title(f'{label} Signal with Peaks')
    plt.xlabel('Index')
    plt.ylabel('Amplitude')
    plt.legend()
    plt.grid(True)
    plt.show()

def analyze_and_plot(filepath, label):
    """Load data, find peaks and minima, and plot the results."""
    data = load_data(filepath)
    peaks, minima = find_peaks_and_minima(data)
    plot_signal_with_peaks(data, peaks, minima, label)

# Analyze and plot for both datasets
analyze_and_plot('/content/Data_1.txt', 'Data_1')
analyze_and_plot('/content/Data_2.txt', 'Data_2')

