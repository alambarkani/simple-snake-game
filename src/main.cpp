#include <SFML/Graphics.hpp>
#include <cmath>

int main()
{
	sf::RenderWindow window(sf::VideoMode({1280, 600}), "Snake Game");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	// Player Sprite
	sf::RectangleShape player({20.f, 20.f});
	player.setFillColor(sf::Color::Green);
	player.setOutlineColor(sf::Color::Black);
	player.setOutlineThickness(1.f);
	player.setPosition({100.f, 100.f});

	player.setOrigin({25.f, 25.f});

	const float playerSpeed = 100.f;

	sf::Clock clock;

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		sf::Time elapsed = clock.restart();
		float deltaTime = elapsed.asSeconds();

		sf::Vector2f velocity{0.f, 0.f};

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			velocity.y -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			velocity.y += 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			velocity.x -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			velocity.x += 1.f;
		}

		if (velocity.x != 0.f && velocity.y != 0.f)
			velocity /= std::sqrt(2.f);

		player.move(velocity * deltaTime * playerSpeed);

		window.clear(sf::Color::White);
		window.draw(player);
		window.display();
	}

	return 0;
}