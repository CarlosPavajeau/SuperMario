#include "GameEngine.h"

namespace GameEngine
{
    std::vector<std::string> Split(const std::string& s, char delim)
    {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim))
            elems.push_back(item);
        return elems;
    }

    float ToFloat(const std::string& str)
    {
        return std::stof(str);
    }

    int ToInt(const std::string& str)
    {
        return std::stoi(str);
    }

    bool ToBool(const std::string& str)
    {
        if (str == "true" || str == "True" || str == "TRUE")
            return true;
        return false;
    }

    namespace math
    {
        int sign(float value)
        {
            return (value > 0) - (value < 0);
        }
    }
    void DrawLinearSprite_v(sf::Sprite sprite, const sf::Rect<int>& draw_area, sf::RenderWindow* render_window)
    {
        if (!draw_area.height)
            return;

        int k = draw_area.height / std::abs(sprite.getTextureRect().height);
        int off_set = draw_area.height % std::abs(sprite.getTextureRect().height);

        for (int i = 0; i < k; ++i)
        {
            sprite.setPosition(draw_area.left, i * 32.f + draw_area.top);
            render_window->draw(sprite);
        }
        auto new_rect = sprite.getTextureRect();
        new_rect.height = off_set;
        sprite.setTextureRect(new_rect);
        sprite.setPosition(draw_area.left, k * 32.f + draw_area.top);
        render_window->draw(sprite);
    }

    void DrawLinearSprite_h(sf::Sprite sprite, const sf::Rect<int>& draw_area, sf::RenderWindow* render_window)
    {
        if (!draw_area.width)
            return;

        int k = draw_area.width / std::abs(sprite.getTextureRect().width);
        int off_set = draw_area.width % std::abs(sprite.getTextureRect().width);

        for (int i = 0; i < k; ++i)
        {
            sprite.setPosition(i * 32.f + draw_area.left, draw_area.top);
            render_window->draw(sprite);
        }
        auto new_rect = sprite.getTextureRect();
        new_rect.width = off_set;
        sprite.setTextureRect(new_rect);
        sprite.setPosition(k * 32.f + draw_area.left, draw_area.top);
        render_window->draw(sprite);
    }

    Vector CollsionResponse(const Rect& own_rect, const Vector& own_speed, const Rect& other_rect, const Vector& other_speed, const float delta_time, CollisionTag& collision_tag)
    {
        Vector new_pos = own_rect.LeftTop();

        Rect intersection = other_rect.GetIntersection(own_rect);
        Vector delta_speed = own_speed - other_speed;

        enum Axis
        {
            None, 
            Vertical,
            Horizontal
        } axis = None;

        if (!intersection.Width || !intersection.Height)
            return new_pos;

        float dt = 0;

        if (delta_speed.X && delta_speed.Y)
        {
            float dx_t = intersection.Width / std::abs(delta_speed.X);
            float dy_t = intersection.Height / std::abs(delta_speed.Y);

            dt = std::min(dx_t, dy_t);

            axis = (dx_t > dy_t) ? Vertical : Horizontal;
        }
        else if (delta_speed.X)
        {
            dt = intersection.Width / std::abs(delta_speed.X);
            axis = Horizontal;
        }
        else if (delta_speed.Y)
        {
            dt = intersection.Height / std::abs(delta_speed.Y);
            axis = Vertical;
        }

        if (dt > delta_time)
            dt = delta_time;

        new_pos -= dt * delta_speed;

        if (axis == Vertical)
        {
            if (intersection.Top == other_rect.Top)
                collision_tag |= CollisionTag::Floor;
            else if (intersection.Bottom() == other_rect.Bottom())
                collision_tag |= CollisionTag::Cell;
        }
        else if (axis == Horizontal)
        {
            if (intersection.Left == other_rect.Left)
                collision_tag |= CollisionTag::Right;
            else if (intersection.Right() == other_rect.Right())
                collision_tag |= CollisionTag::Left;
        }

        return new_pos;
    }
}