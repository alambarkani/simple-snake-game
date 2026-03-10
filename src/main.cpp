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
	std::vector<sf::RectangleShape> snake;
	sf::RectangleShape head({20.f, 20.f});
	head.setFillColor(sf::Color::Green);
	head.setOutlineColor(sf::Color::Black);
	head.setOutlineThickness(1.f);
	head.setPosition({100.f, 100.f});

	head.setOrigin({10.f, 10.f});
	snake.push_back(head);

	sf::Vector2u windowSize = window.getSize();

	sf::Clock clock;

	sf::Vector2f velocity{0.f, 0.f};

	auto direction = Direction::Right;
	auto currentMoveDirection = Direction::Right;
	sf::Time moveInterval = sf::seconds(0.15f);
	sf::Time foodSpawnInterval = sf::seconds(1.f);
	sf::Time moveAccumulator;
	sf::Time foodSpawnAccumulator;

	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->code == sf::Keyboard::Key::D && (currentMoveDirection == Direction::Up || currentMoveDirection == Direction::Down))
				{
					direction = Direction::Right;
				}

				if (keyPressed->code == sf::Keyboard::Key::A && (currentMoveDirection == Direction::Up || currentMoveDirection == Direction::Down))
				{
					direction = Direction::Left;
				}

				if (keyPressed->code == sf::Keyboard::Key::W && (currentMoveDirection == Direction::Left || currentMoveDirection == Direction::Right))
				{
					direction = Direction::Up;
				}

				if (keyPressed->code == sf::Keyboard::Key::S && (currentMoveDirection == Direction::Left || currentMoveDirection == Direction::Right))
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
					float x = static_cast<float>((std::rand() % (windowSize.x / 20)) * 20);
					float y = static_cast<float>((std::rand() % (windowSize.y / 20)) * 20);
					f.shape.setPosition({x, y});
					f.isActive = true;
					break;
				}
			}
		}

		moveAccumulator += elapsed;
		while (moveAccumulator >= moveInterval)
		{
			moveAccumulator -= moveInterval;
			
			sf::Vector2f oldTailPos = snake.back().getPosition();

			for (size_t i = snake.size() - 1; i > 0; --i)
			{
				snake[i].setPosition(snake[i - 1].getPosition());
			}
			snake[0].move(GetVelocity(direction, 20.f));
			currentMoveDirection = direction;

			sf::FloatRect bounds = snake[0].getGlobalBounds();
			if (bounds.position.x < 0.f)
			{
				snake[0].setPosition({static_cast<float>(windowSize.x) - 10.f, snake[0].getPosition().y});
			}
			else if (bounds.position.x + bounds.size.x > windowSize.x)
			{
				snake[0].setPosition({10.f, snake[0].getPosition().y});
			}

			bounds = snake[0].getGlobalBounds();
			if (bounds.position.y < 0.f)
			{
				snake[0].setPosition({snake[0].getPosition().x, static_cast<float>(windowSize.y) - 10.f});
			}
			else if (bounds.position.y + bounds.size.y > windowSize.y)
			{
				snake[0].setPosition({snake[0].getPosition().x, 10.f});
			}

			for (size_t i = 1; i < snake.size(); ++i)
			{
				if (snake[0].getPosition() == snake[i].getPosition())
				{
					snake.resize(1);
					break;
				}
			}

			for (auto &f : foods)
			{
				if (f.isActive)
				{
					sf::Vector2f diff = snake[0].getPosition() - f.shape.getPosition();
					if ((diff.x * diff.x + diff.y * diff.y) < 100.f)
					{
						f.isActive = false;
						sf::RectangleShape newSegment({20.f, 20.f});
						newSegment.setFillColor(sf::Color::Green);
						newSegment.setOutlineColor(sf::Color::Black);
						newSegment.setOutlineThickness(1.f);
						newSegment.setOrigin({10.f, 10.f});
						newSegment.setPosition(oldTailPos);
						snake.push_back(newSegment);
					}
				}
			}
		}

		std::cout << "\rPlayer size: " << snake.size() << ", Pos: " << snake[0].getPosition().x << ", " << snake[0].getPosition().y << "   " << std::flush;

		window.clear(sf::Color::White);
		for (const auto& segment : snake)
		{
			window.draw(segment);
		}
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