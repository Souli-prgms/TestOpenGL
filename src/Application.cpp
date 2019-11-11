#include "Renderer.h"

int main(void)
{
	Renderer* viewer = new Renderer();
	viewer->run();

	delete viewer;
}