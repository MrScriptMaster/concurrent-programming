
#include "app.h"
#include <exception>
#include <iostream>
#include <string>

namespace {
    Tools::Application *app = NULL;

    class BadInit : public std::bad_exception {
            std::string message;
    public:
            BadInit() 
            : std::bad_exception()
            {
                    message = "application is not initialized";
            }
            const char* what() const noexcept {
                    return message.c_str();
            }
    }; // class BadInit
}   // namespace

using namespace Tools;

/* for debug
Tools::Application* Tools::app_init() {
	return NULL;
}//*/

int main(int argc, char** argv)
{
	int status = 0;
	try {	
		app = app_init();
		if (app) {
			app->start();
		}
		else {
			throw new BadInit;
		}
		app->stop();
	}
	catch (const BadInit* e) {
		std::cout << __FILE__ << " (" << __LINE__ << ")" << ":" << __FUNCTION__   
		          << ": " << e->what() << std::endl;
		status = -1;
	}
	catch (const std::exception& e) {
		std::cout << __FILE__ << " (" << __LINE__ << ")" << ":" << __FUNCTION__   
		          << ": " << e.what() << std::endl;
		status = -1;
	}
	catch (...) {
		std::cout << __FILE__ << " (" << __LINE__ << ")" << ":" << __FUNCTION__   
		          << ": " << "EXCEPTION" << std::endl;
		status = -1;
	}
	if (app) {
		delete app;
	}
	return status;
}
