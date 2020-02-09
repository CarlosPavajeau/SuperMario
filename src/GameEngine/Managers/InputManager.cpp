#include "InputManager.h"
#include "GameEngine.h"

namespace GameEngine
{
	namespace Managers
	{
		using namespace GameEngine::Geometry;

		InputManager::InputManager()
		{
			m_keys_prev_ptr = &m_keys_prev;
			m_keys_now_ptr = &m_keys_now;
			m_jsk_btns_prev_ptr = &m_jsk_btns_prev;
			m_jsk_btns_now_ptr = &m_jsk_btns_now;

			for (auto& ax : m_axis_keys)
				ax = sf::Keyboard::Key::Unknown;
		}

		void InputManager::RegisterKey(const sf::Keyboard::Key& key)
		{
			m_keys_prev.insert({ key, false });
			m_keys_now.insert({ key, false });
		}

		void InputManager::UnregisterKey(const sf::Keyboard::Key& key)
		{
			m_keys_prev.erase(m_keys_prev.find(key));
			m_keys_now.erase(m_keys_now.find(key));
		}

		void InputManager::RegisterJoysticButton(int index)
		{
			m_jsk_btns_prev.insert({ index, false });
			m_jsk_btns_now.insert({ index, false });
		}

		bool InputManager::IsKeyJustPressed(const sf::Keyboard::Key& key) const
		{
			if (m_keys_prev_ptr->count(key))
				if (!(*m_keys_prev_ptr)[key] && (*m_keys_now_ptr)[key])
					return true;
			return false;
		}

		bool InputManager::IsKeyJustReleased(const sf::Keyboard::Key& key) const
		{
			if (m_keys_prev_ptr->count(key))
				if ((*m_keys_prev_ptr)[key] && !(*m_keys_now_ptr)[key])
					return true;
			return false;
		}

		bool InputManager::IsKeyPressed(const sf::Keyboard::Key& key) const
		{
			if (m_keys_now_ptr->count(key))
				return (*m_keys_now_ptr)[key];
			return false;
		}

		Vector InputManager::GetXYAxis() const
		{
			Vector value;
			if (sf::Joystick::isConnected(0))
			{
				value.X = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) / 100.f;
				value.Y = -sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) / 100.f;
				value.X = math::sens(value.X, 0.5f);
				value.Y = math::sens(value.Y, 0.5f);
			}

			if (m_axis_keys[0] != -1 && sf::Keyboard::isKeyPressed(m_axis_keys[0])) value.Y = -1;
			if (m_axis_keys[1] != -1 && sf::Keyboard::isKeyPressed(m_axis_keys[1])) value.X = 1;
			if (m_axis_keys[2] != -1 && sf::Keyboard::isKeyPressed(m_axis_keys[2])) value.Y = 1;
			if (m_axis_keys[3] != -1 && sf::Keyboard::isKeyPressed(m_axis_keys[3])) value.X = -1;

			return value;
		}

		bool InputManager::IsButtonPressed(const std::string& button) const
		{
			if (m_btn_to_key.count(button) && IsKeyPressed(m_btn_to_key.at(button)))
				return true;
			if (m_jsk_btn_to_key.count(button) && IsJoystickButtonPressed(m_jsk_btn_to_key.at(button)))
				return true;
			return false;
		}

		bool InputManager::IsButtonDown(const std::string& button) const
		{
			if (m_btn_to_key.count(button) && IsKeyJustPressed(m_btn_to_key.at(button)))
				return true;
			if (m_jsk_btn_to_key.count(button) && IsJoystickButtonJustPressed(m_jsk_btn_to_key.at(button)))
				return true;
			return false;
		}

		bool InputManager::IsButtonUp(const std::string& button) const
		{
			if (m_btn_to_key.count(button) && IsKeyJustReleased(m_btn_to_key.at(button)))
				return true;
			if (m_jsk_btn_to_key.count(button) && IsJoystickButtonJustPressed(m_jsk_btn_to_key.at(button)))
				return true;
			return false;
		}

		sf::Keyboard::Key InputManager::ToKey(const std::string& str)
		{
			if (str.length() == 1 && str[0] >= 'A' && str[0] <= 'Z')
				return static_cast<sf::Keyboard::Key>(str[0] - 65);

			static const std::map<std::string, sf::Keyboard::Key> key_map =
			{
			{ "Left", sf::Keyboard::Left },
			{ "Right", sf::Keyboard::Right },
			{ "Up", sf::Keyboard::Up },
			{ "Down", sf::Keyboard::Down },
			{ "Space", sf::Keyboard::Space },
			{ "LShift", sf::Keyboard::LShift },
			{ "Enter", sf::Keyboard::Enter },
			{ "Return", sf::Keyboard::Return },
			};

			if (key_map.find(str) != key_map.end())
				return key_map.at(str);
			return sf::Keyboard::Unknown;
		}

		void InputManager::SetupButton(const std::string& button, const std::vector<std::string>& keys)
		{
			static const std::unordered_map<std::string, int> special_keys =
			{
				{ "Vertical-",0},
				{ "Horizontal+",1 },
				{ "Vertical+",2 },
				{ "Horizontal-",3 }
			};

			if (special_keys.count(button))
			{
				m_axis_keys[special_keys.at(button)] = ToKey(keys[0]);
				return;
			}

			for (auto key : keys)
			{
				if (key.front() == '[' && key.back() == ']') //joystick btn
				{
					int index = ToInt(key.substr(1, key.length() - 2));
					RegisterJoysticButton(index);
					m_jsk_btn_to_key[button] = index;
				}
				else
				{
					sf::Keyboard::Key pkey = ToKey(key);
					RegisterKey(pkey);
					m_btn_to_key[button] = pkey;
				}
			}
		}

		void InputManager::Update(int delta_time)
		{
			std::swap(m_keys_now_ptr, m_keys_prev_ptr);
			for (auto& key : *m_keys_now_ptr)
				key.second = sf::Keyboard::isKeyPressed(key.first);

			std::swap(m_jsk_btns_now, m_jsk_btns_prev);
			for (auto& btn : *m_jsk_btns_now_ptr)
				btn.second = sf::Joystick::isButtonPressed(0, btn.first);
		}

		bool InputManager::IsJoystickButtonPressed(int index) const
		{
			if (m_jsk_btns_now_ptr->count(index))
				return (*m_jsk_btns_now_ptr)[index];
			return false;
		}

		bool InputManager::IsJoystickButtonJustPressed(int index) const
		{
			if (m_jsk_btns_prev_ptr->count(index))
				if (!(*m_jsk_btns_prev_ptr)[index] && (*m_jsk_btns_now_ptr)[index])
					return true;
			return false;
		}

		bool InputManager::IsJoystickButtonJustReleased(int index) const
		{
			if (m_jsk_btns_prev_ptr->count(index))
				if ((*m_jsk_btns_prev_ptr)[index] && !(*m_jsk_btns_now_ptr)[index])
					return true;
			return false;
		}

	}
}