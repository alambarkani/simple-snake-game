#include <SFML/Graphics.hpp>
#include <iostream>

enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

sf::Vector2f GetVelocity(Direction direction, float cellSize)
{
	switch (direction)
	{
	case Direction::Up:
		return {0.f, -cellSize};
	case Direction::Down:
		return {0.f, cellSize};
	case Direction::Left:
		return {-cellSize, 0.f};
	case Direction::Right:
		return {cellSize, 0.f};
	default:
		return {0.f, 0.f};
	}
}

struct food
{
	sf::CircleShape shape;
	bool isActive{false};

	food() : shape(10.f)
	{
		shape.setFillColor(sf::Color::Red);
		shape.setOrigin({10.f, 10.f});
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode({1280, 600}), "Snake Game");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	std::vector<food> foods(5);

	// Player Sprite
	sf::RectangleShape player({20.f, 20.f});
	player.setFillColor(sf::Color::Green);
	player.setOutlineColor(sf::Color::Black);
	player.setOutlineThickness(1.f);
	player.setPosition({100.f, 100.f});

	player.setOrigin({10.f, 10.f});

	sf::Vector2u windowSize = window.getSize();

	sf::Clock clock;

	sf::Vector2f velocity{0.f, 0.f};

	auto direction = Direction::Right;
	sf::Time moveInterval = sf::seconds(0.15f);
	sf::Time foodSpawnInterval = sf::seconds(1.f);
	sf::Time moveAccumulator;
	sf::Time foodSpawnAccumulator;

	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();

		sf::FloatRect playerBounds = player.getGlobalBounds();
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->code == sf::Keyboard::Key::D && (direction == Direction::Up || direction == Direction::Down))
				{
					direction = Direction::Right;
				}

				if (keyPressed->code == sf::Keyboard::Key::A && (direction == Direction::Up || direction == Direction::Down))
				{
					direction = Direction::Left;
				}

				if (keyPressed->code == sf::Keyboard::Key::W && (direction == Direction::Left || direction == Direction::Right))
				{
					direction = Direction::Up;
				}

				if (keyPressed->code == sf::Keyboard::Key::S && (direction == Direction::Left || direction == Direction::Right))
				{
					direction = Direction::Down;
				}
			}
		}

		foodSpawnAccumulator += elapsed;
		if (foodSpawnAccumulator >= foodSpawnInterval)
		{
			foodSpawnAccumulator -= foodSpawnInterval;
			for (auto &f : foods)
			{
				if (!f.isActive)
				{
					f.shape.setPosition({static_cast<float>(std::rand() % windowSize.x), static_cast<float>(std::rand() % windowSize.y)});
					f.isActive = true;
					break;
				}
			}
		}

		moveAccumulator += elapsed;
		while (moveAccumulator >= moveInterval)
		{
			moveAccumulator -= moveInterval;
			player.move(GetVelocity(direction, 20.f));
		}

		if (playerBounds.position.x <= 0.f)
		{
			player.setPosition({windowSize.x - 20.f, player.getPosition().y});
		}
		if (playerBounds.position.y <= 0.f)
		{
			player.setPosition({player.getPosition().x, windowSize.y - 20.f});
		}
		if (playerBounds.position.y + playerBounds.size.y >= windowSize.y)
		{
			player.setPosition({player.getPosition().x, 20.f});
		}
		if (playerBounds.position.x + playerBounds.size.x >= windowSize.x)
		{
			player.setPosition({20.f, player.getPosition().y});
		}

		std::cout << "Player Velocity: " << GetVelocity(direction, 20.f).x << ", " << GetVelocity(direction, 20.f).y << std::endl;

		window.clear(sf::Color::White);
		window.draw(player);
		for (const auto &f : foods)
		{
			if (f.isActive)
			{
				window.draw(f.shape);
			}
		}
		window.display();
	}

	return 0;
}