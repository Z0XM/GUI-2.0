#include "src/Renderer.hpp"

int main()
{
	Renderer app;
	while (app.isRunning()) {
		app.update();
		app.render();
	}
}