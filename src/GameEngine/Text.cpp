#include "Text.h"


namespace GameEngine
{
	FlowText::FlowText(const sf::Font& font, bool self_remove)
	{
		text.setFont(font);
		text.setFillColor(sf::Color::Black);
		text.setCharacterSize(20);
		text.setStyle(sf::Text::Bold);
		flashing = false;
		this->self_remove = self_remove;
	}

	void FlowText::SetTextColor(const sf::Color& color)
	{
		text.setFillColor(color);
	}

	void FlowText::SetTextSize(int size)
	{
		text.setCharacterSize(size);
	}

	void FlowText::Splash(const Vector& pos, const std::string& text)
	{
		flashing = true;
		SetPosition(pos);
		this->text.setString(text);
		offset.X = offset.Y = 0;
		time = 0;
	}

	void FlowText::Update(int delta_time)
	{
		if (flashing)
		{
			time += delta_time;
			offset.X = time * 0.03f * splash_vector.X;
			offset.Y = time * 0.03f * splash_vector.Y;

			offset.X *= 2;
			color = time * 0.2f;
			if (color >= 255)
				flashing = false;

			const sf::Color& color = text.getFillColor();

			text.setFillColor(sf::Color(color.r, color.g, color.b, 255 - this->color));
		}
		else if (self_remove)
		{
			if (GetParent())
				GetParent()->RemoveObject(this);
			else
				delete this;
		}
	}

	void FlowText::Draw(sf::RenderWindow* window)
	{
		if (flashing)
		{
			text.setPosition(GetPosition() + offset);
			window->draw(text);
		}
	}

	bool FlowText::IsFlashing() const
	{
		return flashing;
	}

	void FlowText::SetSplashVector(const Vector& vector)
	{
		splash_vector = vector;
	}

	FlowText* FlowText::Clone() const
	{
		FlowText* flow_text = new FlowText(*text.getFont());
		flow_text->text = text;
		flow_text->splash_vector = splash_vector;
		return flow_text;
	}

	Label::Label()
	{
		Init();
	}

	Label::Label(const std::string& str)
	{
		Init();
		SetString(str);
	}

	Label::Label(const sf::Sprite& sprite)
	{
		Init();
		SetSprite(sprite);
	}

	void Label::Init()
	{
		SetName("Label");
		SetFontColor(sf::Color::Black);
		sprite.setColor({ 255, 255, 255, 50 });
	}

	void Label::SetSprite(const sf::Sprite& sprite)
	{
		this->sprite = sprite;
	}

	void Label::SetString(const std::string& str)
	{
		text.setString(str);
	}

	void Label::SetTextAlign(TextAling value)
	{
		text_align = value;
	}

	void Label::SetFontColor(const sf::Color& color)
	{
		text.setFillColor(color);
	}

	void Label::SetFontSize(int size)
	{
		text.setCharacterSize(size);
	}

	void Label::SetOutlineColor(const sf::Color& color)
	{
		shape.setOutlineColor(color);
	}

	void Label::SetFillColor(const sf::Color& color)
	{
		shape.setFillColor(color);
	}

	void Label::SetOutlineThickness(int value)
	{
		shape.setOutlineThickness(value);
	}

	void Label::SetBounds(int x, int y, int w, int h)
	{
		SetPosition(x, y);
		rect = Rect(x, y, w, h);

		shape.setPosition(x, y);
		shape.setSize(sf::Vector2f(w, h));

		if (sprite.getTexture())
		{
			sprite.setPosition(sf::Vector2f(x, y));
			sprite.setScale(w / sprite.getTextureRect().width, h / sprite.getTextureRect().height);
		}
	}

	void Label::SetFontName(const sf::Font& font)
	{
		text.setFont(font);
	}

	void Label::SetFontStyle(sf::Uint32 style)
	{
		text.setStyle(style);
	}

	bool Label::Contains(const Vector& point) const
	{
		return rect.IsContain(point);
	}

	Label* Label::Clone() const
	{
		Label* new_label = new Label();

		new_label->text = text;
		new_label->sprite = sprite;
		new_label->rect = rect;
		new_label->shape = shape;
		new_label->text_align = text_align;
		Rect bounds = GetBounds();
		new_label->SetBounds(bounds.Left, bounds.Top, bounds.Width, bounds.Height);

		return new_label;
	}

	void Label::Draw(sf::RenderWindow* window)
	{
		window->draw(shape);

		if (sprite.getTexture())
		{
			sprite.setPosition(GetPosition());
			window->draw(sprite);
		}

		if (!text.getString().isEmpty())
		{
			if (text_align == TextAling::Center)
				text.setPosition(GetPosition() + rect.Size() / 2 - Vector(text.getGlobalBounds().width, text.getGlobalBounds().height) / 2);
			else if (text_align == TextAling::Left)
				text.setPosition(GetPosition().X, GetPosition().Y);

			window->draw(text);
		}
	}

	void Label::OnPropertySet(const std::string& name)
	{
		GameObject::OnPropertySet(name);

		if (name == "Text")
			SetString(GetProperty("Text").AsString());
	}

	void Label::OnActivated()
	{
		if (GetProperty("X").IsValid())
		{
			SetBounds(
				GetProperty("X").AsFloat(),
				GetProperty("Y").AsFloat(),
				GetProperty("Width").AsFloat(),
				GetProperty("Height").AsFloat()
			);

			SetString(GetProperty("Text").AsString());
		}

		if (GetProperty("Hided").IsValid() &&
			GetProperty("Hided").AsBool())
			Hide();
	}
}