#include <cstdio>
#include <sys/time.h>

extern "C" {
    void sayHello();
    int myPuts(const char* s, int len);
    struct timeval myGTOD();
}

int main() {
    sayHello();
    const char* msg = "Hello from myPuts\n";
    myPuts(msg, 19);

    struct timeval time = myGTOD();
    printf("Seconds: %ld, Microseconds: %ld\n", time.tv_sec, time.tv_usec);

    return 0;
}

