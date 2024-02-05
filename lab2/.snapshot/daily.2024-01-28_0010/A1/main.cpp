extern "C" void sayHello();
extern "C" int myPuts(const char* s, int len);

int main() {
    sayHello();
    const char* message = "Hello from myPuts!";
    myPuts(message, 19);
    return 0;
}
