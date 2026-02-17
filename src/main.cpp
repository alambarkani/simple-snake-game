#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode({200, 200}), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	sf::RectangleShape rectangle({100.f, 100.f});
	rectangle.setFillColor(sf::Color::Red);
	rectangle.setOrigin({50.f, 50.f});

	float rotationSpeed = 100.f;
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		float dt = deltaTime.asSeconds();
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		rectangle.rotate(sf::degrees(rotationSpeed * dt));

		window.clear();
		window.draw(shape);
		window.draw(rectangle);
		window.display();
	}
}
