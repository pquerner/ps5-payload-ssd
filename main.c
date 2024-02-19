#include <stdio.h>
#include <string.h>
#include <math.h>

#include "jailbreak.c"
#include "io_benchmark.c"

#define DIRECTORY "/user/temp"
#define FILE_COUNT 1
#define NUM_RUNS 1
#define FILE_SIZE (1024 * 1024 * 100) // 100 MB

typedef struct notify_request
{
    char useless1[45];
    char message[3075];
} notify_request_t;

int sceKernelSendNotificationRequest(int, notify_request_t *, size_t, int);

void sendNotification(char *message)
{
    notify_request_t req;

    bzero(&req, sizeof req);
    strncpy(req.message, message, sizeof req.message);
    sceKernelSendNotificationRequest(0, &req, sizeof req, 0);
}

void progress_callback(double progress)
{
    if (fmod(progress, 10.0) == 0)
    {
        char _progress[100];
        sprintf(_progress, "Progress: %.2f%%\n", progress);
        sendNotification(_progress);
    }
}

void secondsToHoursMinutesSeconds(int totalSeconds, int *hours, int *minutes, int *seconds)
{
    *hours = totalSeconds / 3600; // 3600 seconds in an hour
    totalSeconds %= 3600;
    *minutes = totalSeconds / 60; // 60 seconds in a minute
    *seconds = totalSeconds % 60;
}

int main()
{
    // Escape Sandbox
    pid_t pid = getpid();
    jailbreak_process(pid, true);

    sendNotification("Testing internal drive speed. Hold on please.");
    BenchmarkResult result = run_benchmark(DIRECTORY, FILE_COUNT, FILE_SIZE, NUM_RUNS, progress_callback);

    int totalSeconds;
    int hours, minutes, seconds;

    const char *averageWriteTimeDesc = "Average write time";
    const char *totalWriteTimeDesc = "Total write time testing 5 times";
    const char *throughputDesc = "Throughput";

    char throughputStr[50];
    snprintf(throughputStr, sizeof(throughputStr), "%.2f", result.throughput);

    char averageWriteTimeOutput[100];
    char totalWriteTimeOutput[100];
    char throughputOutput[100];

    secondsToHoursMinutesSeconds(result.averageWriteTime, &hours, &minutes, &seconds);
    sprintf(averageWriteTimeOutput, "%s: Hours: %d, Minutes: %d, Seconds: %d\n", averageWriteTimeDesc, hours, minutes, seconds);
    secondsToHoursMinutesSeconds(result.totalWriteTime, &hours, &minutes, &seconds);
    sprintf(totalWriteTimeOutput, "%s: Hours: %d, Minutes: %d, Seconds: %d\n", totalWriteTimeDesc, hours, minutes, seconds);
    sprintf(throughputOutput, "%s: %s bytes/s\n", throughputDesc, throughputStr);

    printf("%s\n", averageWriteTimeOutput);
    printf("%s\n", totalWriteTimeOutput);
    printf("%s\n", throughputOutput);

    sendNotification(averageWriteTimeOutput);
    sendNotification(totalWriteTimeOutput);
    sendNotification(throughputOutput);

    return EXIT_SUCCESS;
}