#ifndef APPLICATION_H
#define APPLICATION_H

// The console menu and the user-facing flows of the program.
class Application {
public:
    // Runs the menu loop; returns the exit code.
    int run();

private:
    void printMenu() const;
    void classicFlow(int mode);      // modes 1-4: enter/generate/read, sort, show
    void generateTestFiles();        // mode 5
    void processFile();              // mode 6
    void speedTest();                // mode 7
};

#endif
