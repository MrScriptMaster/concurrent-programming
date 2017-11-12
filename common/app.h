
#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

namespace Tools {

class Application {
	
public:
	explicit Application() {}
	virtual ~Application() {}
	
	virtual void start() const {}
	virtual void stop() const {}
	
}; // class Application

Application* app_init();
	
} // namespace Tools

#endif
