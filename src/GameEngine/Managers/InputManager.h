#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Geometry.h"

#include "SFML/Graphics.hpp"
#include <unordered_map>

namespace GameEngine
{
	namespace Managers
	{
		class InputManager
		{
		public:
			InputManager();

			GameEngine::Geometry::Vector GetXYAxis() const;

			bool IsButtonPressed(const std::string& button) const;
			bool IsButtonDown(const std::string& button) const;
			bool IsButtonUp(const std::string& button) const;
			void SetupButton(const std::string& button, const std::vector<std::string>& keys);
			void Update(int delta_time);

		private:
			std::unordered_map<sf::Keyboard::Key, bool> m_keys_prev, * m_keys_prev_ptr, m_keys_now, * m_keys_now_ptr;
			std::unordered_map<int, bool> m_jsk_btns_prev, * m_jsk_btns_prev_ptr, m_jsk_btns_now, * m_jsk_btns_now_ptr;
			sf::Keyboard::Key m_axis_keys[4];
			std::unordered_map<std::string, sf::Keyboard::Key> m_btn_to_key;
			std::unordered_map<std::string, int> m_jsk_btn_to_key;
			bool IsKeyJustPressed(const sf::Keyboard::Key& key) const;
			bool IsKeyJustReleased(const sf::Keyboard::Key& key) const;
			bool IsKeyPressed(const sf::Keyboard::Key& key) const;
			bool IsJoystickButtonPressed(int index) const;
			bool IsJoystickButtonJustPressed(int index) const;
			bool IsJoystickButtonJustReleased(int index) const;
			void RegisterKey(const sf::Keyboard::Key& key);
			void UnregisterKey(const sf::Keyboard::Key& key);
			void RegisterJoysticButton(int index);
			sf::Keyboard::Key ToKey(const std::string& str);
		};
	}
}

#endif // !INPUT_MANAGER_H
