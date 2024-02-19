#ifndef IO_BENCHMARK_H
#define IO_BENCHMARK_H

typedef struct
{
    double averageWriteTime; // Average time taken to write a file (in milliseconds)
    double totalWriteTime;   // Total time taken to write all files (in milliseconds)
    double throughput;       // Throughput in bytes per second
} BenchmarkResult;

void create_files(const char *fileLocation, int numFiles, int fileSizeBytes);
void write_file_with_callback(const char *filename, int fileSizeBytes, void (*callback)(double progress));
double write_files_with_callback(const char *fileLocation, int numFiles, int fileSizeBytes, void (*callback)(double progress));
void delete_files(const char *fileLocation, int numFiles);
BenchmarkResult run_benchmark(const char *fileLocation, int numFiles, int fileSizeBytes, int numRuns, void (*callback)(double progress));
void benchmark_with_callback(const char *fileLocation, int numFiles, int fileSizeBytes, void (*callback)(double progress));

#endif /* IO_BENCHMARK_H */