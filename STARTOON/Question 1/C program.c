#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_DATA_SIZE 1000

typedef struct {
    double *data;
    int size;
} DataSet;

typedef struct {
    int *indices;
    int count;
} Peaks;

DataSet load_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    DataSet dataset;
    dataset.data = (double *)malloc(MAX_DATA_SIZE * sizeof(double));
    dataset.size = 0;

    while (fscanf(file, "%lf", &dataset.data[dataset.size++]) != EOF);
    fclose(file);
    
    return dataset;
}

Peaks find_peaks(double *data, int size, int window, double threshold) {
    Peaks peaks;
    peaks.indices = (int *)malloc(MAX_DATA_SIZE * sizeof(int));
    peaks.count = 0;

    for (int i = window; i < size - window; i++) {
        bool is_peak = true;
        for (int j = 1; j <= window; j++) {
            if (data[i] <= data[i - j] || data[i] <= data[i + j]) {
                is_peak = false;
                break;
            }
        }
        if (is_peak && data[i] > threshold) {
            peaks.indices[peaks.count++] = i;
        }
    }

    return peaks;
}

void plot_data(double *data, int size, Peaks peaks, Peaks minima, const char *label) {
    int plot_width = 70;
    int plot_height = 20;
    double max_val = data[0], min_val = data[0];
    char plot[plot_height][plot_width];

    for (int i = 1; i < size; i++) {
        if (data[i] > max_val) max_val = data[i];
        if (data[i] < min_val) min_val = data[i];
    }

    for (int i = 0; i < plot_height; i++) {
        for (int j = 0; j < plot_width; j++) {
            plot[i][j] = ' ';
        }
    }

    for (int i = 0; i < size; i++) {
        int x = (i * plot_width) / size;
        int y = (int)((data[i] - min_val) / (max_val - min_val) * (plot_height - 1));
        plot[plot_height - 1 - y][x] = '*';
    }

    for (int i = 0; i < peaks.count; i++) {
        int x = (peaks.indices[i] * plot_width) / size;
        int y = (int)((data[peaks.indices[i]] - min_val) / (max_val - min_val) * (plot_height - 1));
        plot[plot_height - 1 - y][x] = 'M';
    }

    for (int i = 0; i < minima.count; i++) {
        int x = (minima.indices[i] * plot_width) / size;
        int y = (int)((data[minima.indices[i]] - min_val) / (max_val - min_val) * (plot_height - 1));
        plot[plot_height - 1 - y][x] = 'm';
    }

    printf("\n%s\n", label);
    for (int i = 0; i < plot_height; i++) {
        for (int j = 0; j < plot_width; j++) {
            printf("%c", plot[i][j]);
        }
        printf("\n");
    }
}

void find_and_plot_peaks(DataSet dataset, const char *label) {
    Peaks peaks = find_peaks(dataset.data, dataset.size, 20, 0.5);

    double *inversed_data = (double *)malloc(dataset.size * sizeof(double));
    for (int i = 0; i < dataset.size; i++) {
        inversed_data[i] = -dataset.data[i];
    }

    Peaks minima = find_peaks(inversed_data, dataset.size, 20, 0.5);

    plot_data(dataset.data, dataset.size, peaks, minima, label);

    free(peaks.indices);
    free(minima.indices);
    free(inversed_data);
}

int main() {
    DataSet data1 = load_data("Data_1.txt");
    DataSet data2 = load_data("Data_2.txt");

    find_and_plot_peaks(data1, "Data_1");
    find_and_plot_peaks(data2, "Data_2");

    free(data1.data);
    free(data2.data);

    return 0;
}
