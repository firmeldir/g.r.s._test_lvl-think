#include<SFML/Graphics.hpp>

using namespace sf;

View heroview;


View coordinateforview(float x, float y)
{
	heroview.setCenter(x, y);

	return heroview;
}

