/////////////////////////////////////////////////////////////////////////
// --- create 23.06.2021 author Storchak Vasil (github Who-are-me) --- //
/////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <memory>

#include <kernel/Application.hpp>

class MyApp : public MatrixEngine::Application {
    int frame = 0;

    virtual void onUpdate() override {
        //std::cout << "Update frame: " << frame++ << std::endl;
    }
};

int main() {

    auto myApp = std::make_unique<MyApp>();

    int returnCode = myApp->start(1024, 768, "MY First App");

    std::cin.get();

    return returnCode;
}
