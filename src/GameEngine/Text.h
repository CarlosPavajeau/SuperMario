#ifndef TEXT_H
#define TEXT_H

#include "GameObject.h"

#include <string>

namespace GameEngine
{
	class FlowText : public GameObjects::GameObject
	{
	public:
		FlowText(const sf::Font& font, bool self_remove = false);

		void SetTextColor(const sf::Color& color);
		void SetTextSize(int size);
		void Splash(const Vector& pos, const std::string& text);

		virtual void Update(int delta_time) override;
		virtual void Draw(sf::RenderWindow* window) override;

		bool IsFlashing() const;
		void SetSplashVector(const Vector& vector);
		FlowText* Clone() const;

	private:
		bool self_remove = false;
		sf::Text text;
		Vector offset;
		Vector splash_vector = { 1,-1 };
		float time;
		int color;
		bool flashing;
	};

	class Label : public GameObjects::GameObject
	{
	public:
		enum class TextAling
		{
			Left,
			Center
		};

		Label(); 
		Label(const std::string& str);
		Label(const sf::Sprite& sprite);

		void SetSprite(const sf::Sprite& sprite);
		void SetString(const std::string& str);
		void SetTextAlign(TextAling value);
		void SetFontColor(const sf::Color& color);
		void SetFontSize(int size);
		void SetOutlineColor(const sf::Color& color);
		void SetFillColor(const sf::Color& color);
		void SetOutlineThickness(int value);
		void SetBounds(int x, int y, int w, int h);
		void SetFontName(const sf::Font& font);
		void SetFontStyle(sf::Uint32 style);

		Rect GetBounds() const override { return rect; }
		sf::Sprite& GetSprite() { return sprite; }

		bool Contains(const Vector& point) const;
		Label* Clone() const;

		virtual void Draw(sf::RenderWindow* window) override;
	protected:
		virtual void OnPropertySet(const std::string& name) override;
		virtual void OnActivated() override;
		sf::RectangleShape shape;
	private:
		void Init();
		TextAling text_align = TextAling::Center;
		sf::Sprite sprite;
		Rect rect;
		sf::Text text;
	};
}

#endif // !TEXT_H