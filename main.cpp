#include "Application.h"
#include "Exceptions.h"

#include <exception>
#include <iostream>

int main() {
    try {
        Application app;
        return app.run();
    } catch (const InputEnded&) {
        std::cout << "\nInput ended. Exiting.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nFatal error: " << e.what() << "\n";
        return 1;
    }
}
