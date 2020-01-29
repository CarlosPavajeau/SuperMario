#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <functional>
#include <list>

#include "Geometry.h"

using namespace GameEngine::Geometry;

namespace GameObjects
{
	class Property
	{
	public:
		Property();
		Property(bool bool_value);
		Property(int int_value);
		Property(const std::string& string_value);
		Property(float float_value);
		Property(const Property& property);
		Property(Property&& property);
		~Property();

		Property& operator=(const Property& property);
		Property& operator=(Property&& property);

		bool AsBool() const;
		int AsInt() const;
		float AsFloat() const;
		const std::string& AsString() const;
		bool IsValid() const;
	private:
		union
		{
			int int_data;
			float float_data;
			std::string* string_data;
			bool bool_data;
		};
		enum class Type { NoInit, Bool, Int, Float, String } _type;
	};

	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();

		void SetName(const std::string& name);
		const std::string& GetName() const;
		void SetProperty(const std::string& name, const Property& property);
		Property GetProperty(const std::string& name) const;
		void SetParent(GameObject* game_object);
		GameObject* GetParent() const;

		GameObject* AddObject(GameObject* object);
		GameObject* FindObjectByName(const std::string& name);

		void MoveToBack();
		void MoveToFront();
		void MoveUnderTo(GameObject* obj);

		template <typename T>
		T* FindObjectByName(const std::string& name)
		{
			auto it = std::find_if(_objects.begin(), _objects.end(), [this, &name](const GameObject* obj) -> bool { return obj->GetName() == name;  });
			if (it != _objects.end())
				return dynamic_cast<T*>(*it);
			return nullptr;
		}

		template <typename T>
		T* FindObjectByType()
		{
			for (auto& obj : _objects)
				if (dynamic_cast<T*>(obj) != nullptr)
					return (T*)obj;
			return nullptr;
		}

		template <typename T>
		std::vector<T*> FindObjectsByType()
		{
			std::vector<T*> objects;
			for (auto& obj : _objects)
			{
				if (dynamic_cast<T*>(obj) != nullptr)
					objects.push_back((T*)obj);

				auto objects_temp = obj->FindObjectsByType<T>();
				if (!objects_temp.empty())
					objects.insert(objects.end(), objects_temp.begin(), objects_temp.end());
			}
			return objects;
		}

		template <typename T>
		T* CastTo()
		{
			assert(dynamic_cast<T*>(this));
			return (T*)this;
		}

		template <typename T>
		bool IsTypeOf() const
		{
			return (dynamic_cast<const T*>(this) != nullptr);
		}

		void ForeachObject(std::function<void(GameObject*)> predicate);
		void ForeachObject(std::function<void(GameObject*, bool& need_break)> predicate);
		void RemoveObject(GameObject* obj);
		void Clear();

		static void InvokePreupdateActions();

		virtual void Init() { }
		virtual void Update(int delta_time);
		virtual void Events(const sf::Event& event) { }

		void Enable();
		void Disable();
		bool IsEnabled() const;
		void Hide();
		void Show();
		bool IsVisible() const;
		void TurnOn();
		void TurnOff();

		virtual void Draw(sf::RenderWindow* window);
		virtual void PostDraw(sf::RenderWindow* window);

		const Vector& GetPosition() const;
		void SetPosition(const Vector& point);
		void SetPosition(float x, float y);
		void Move(const Vector& point);

		virtual Rect GetBounds() const;
		virtual void SetBounds(const Rect& rect);

		void SetSize(const Vector& size);
	protected:
		virtual void OnPropertySet(const std::string& name);
		virtual void OnPropertyGet(const std::string& name) const { }
		virtual void OnActivated() { }
		virtual void OnPositionChanged(const Vector& new_pos, const Vector& old_pos) { }

	private:
		std::string _name;
		bool _started = false;
		static std::vector<std::function<void()>> _preupdate_actions;
		std::map<std::string, Property> _properties;
		GameObject* _parent;
		std::list<GameObject*> _objects;
		bool _enable;
		bool _visible;
		Vector _pos, _size;
	};
}
#endif // !GAME_OBJECT_H
