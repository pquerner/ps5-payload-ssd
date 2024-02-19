#include "io_benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

void create_files(const char *fileLocation, int numFiles, int fileSizeBytes)
{
    int i;
    FILE *file;
    char filename[255];

    for (i = 0; i < numFiles; ++i)
    {
        snprintf(filename, sizeof(filename), "%s/file_%d.txt", fileLocation, i);
        file = fopen(filename, "wb");
        fseek(file, fileSizeBytes - 1, SEEK_SET);
        fputc('\0', file);
        fclose(file);
    }
}

void write_file_with_callback(const char *filename, int fileSizeBytes, void (*callback)(double progress))
{
    FILE *file = fopen(filename, "wb");
    int bytesWritten = 0;
    int chunkSize = 1024 * 1024; // 1 MB chunk size
    char data[chunkSize];

    while (bytesWritten < fileSizeBytes)
    {
        fwrite(data, 1, chunkSize, file);
        bytesWritten += chunkSize;

        // Call the progress callback
        if (callback != NULL)
        {
            double progress = (double)bytesWritten / fileSizeBytes * 100.0;
            callback(progress);
        }
    }

    fclose(file);
}

double write_files_with_callback(const char *fileLocation, int numFiles, int fileSizeBytes, void (*callback)(double progress))
{
    clock_t start, end;
    double total_time = 0.0;

    for (int i = 0; i < numFiles; ++i)
    {
        char filename[255];
        snprintf(filename, sizeof(filename), "%s/__filewritespeedtest_%d.txt", fileLocation, i);

        start = time(NULL);

        write_file_with_callback(filename, fileSizeBytes, callback);

        end = time(NULL);
        total_time = difftime(end, start);
    }

    return total_time;
}

void delete_files(const char *fileLocation, int numFiles)
{
    int i;
    char filename[255];

    for (i = 0; i < numFiles; ++i)
    {
        snprintf(filename, sizeof(filename), "%s/__filewritespeedtest_%d.txt", fileLocation, i);
        remove(filename);
    }
}

BenchmarkResult run_benchmark(const char *fileLocation, int numFiles, int fileSizeBytes, int numRuns, void (*callback)(double progress))
{
    double total_write_time = 0.0;

    for (int j = 0; j < numRuns; ++j)
    {
        total_write_time += write_files_with_callback(fileLocation, numFiles, fileSizeBytes, callback);
        delete_files(fileLocation, numFiles);
    }

    BenchmarkResult result;
    result.totalWriteTime = total_write_time;
    result.averageWriteTime = total_write_time / numRuns;
    result.throughput = ((double)(numFiles * fileSizeBytes)) / (result.averageWriteTime / 1000.0); // bytes per second
    return result;
}