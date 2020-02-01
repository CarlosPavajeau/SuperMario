#include "GameObject.h"

namespace GameObjects
{
	Property::Property()
	{
		_type = Type::NoInit;
	}

	Property::Property(const Property& property)
	{
		*this = property;
	}

	Property::Property(Property&& property)
	{
		*this = &property;
	}

	Property::Property(bool bool_value)
	{
		_type = Type::Bool;
		bool_data = bool_value;
	}

	Property::Property(int int_value)
	{
		_type = Type::Int;
		int_data = int_value;
	}

	Property::Property(const std::string& string_value)
	{
		_type = Type::String;
		string_data = new std::string(string_value);
	}

	Property::Property(float float_value)
	{
		_type = Type::Float;
		bool_data = float_value;
	}

	Property::~Property()
	{
		if (_type == Type::String)
			delete string_data;
	}

	Property& Property::operator=(const Property& property)
	{
		_type = property._type;

		switch (_type)
		{
		case Type::Bool:
			bool_data = property.bool_data;
			break;
		case Type::Int:
			int_data = property.int_data;
			break;
		case Type::Float:
			float_data = property.float_data;
			break;
		case Type::String:
			string_data = new std::string(*property.string_data);
			break;
		default:
			break;
		}

		return *this;
	}

	Property& Property::operator=(Property&& property)
	{
		*this = property;

		if (_type == Type::String)
			property.string_data = nullptr;

		return *this;
	}

	bool Property::AsBool() const
	{
		assert(_type == Type::Bool);
		return bool_data;
	}

	int Property::AsInt() const
	{
		assert(_type == Type::Int);
		return int_data;
	}

	float Property::AsFloat() const
	{
		assert(_type == Type::Float);
		return float_data;
	}

	const std::string& Property::AsString() const
	{
		assert(_type == Type::String);
		return *string_data;
	}

	bool Property::IsValid() const
	{
		return _type != Type::NoInit;
	}

	GameObject::GameObject()
	{
		_enable = _visible = true;
		_parent = nullptr;
	}

	GameObject::~GameObject()
	{
		for (auto& obj : _objects)
			delete obj;
		_objects.clear();
	}

	void GameObject::SetName(const std::string& name)
	{
		_name = name;
	}

	const std::string& GameObject::GetName() const
	{
		return _name;
	}

	void GameObject::SetProperty(const std::string& name, const Property& property)
	{
		_properties[name] = property;
		OnPropertySet(name);
	}

	Property GameObject::GetProperty(const std::string& name) const
	{
		OnPropertyGet(name);
		return const_cast<GameObject*>(this)->_properties[name];
	}

	void GameObject::SetParent(GameObject* game_object)
	{
		_parent = game_object;
	}

	GameObject* GameObject::GetParent() const
	{
		return _parent;
	}

	GameObject* GameObject::AddObject(GameObject* object)
	{
		_objects.push_back(object);
		object->SetParent(this);
		object->OnActivated();

		if (_started)
		{
			object->_started = true;
			object->Init();
		}

		return object;
	}

	GameObject* GameObject::FindObjectByName(const std::string& name)
	{
		auto it = std::find_if(_objects.begin(), _objects.end(), [this, &name](const GameObject* obj) -> bool { return obj->GetName() == name;  });
		if (it != _objects.end())
			return *it;
		return nullptr;
	}

	void GameObject::MoveToBack()
	{
		if (GameObject* parent = GetParent())
		{
			auto move_to_back_action = [this, parent]()
			{
				auto list = &(parent->_objects);
				auto it = std::find(list->begin(), list->end(), this);

				assert(*it == this);

				auto tmp = *it;
				it = list->erase(it);
				list->push_front(tmp);
			};

			_preupdate_actions.push_back(move_to_back_action);
		}
	}

	void GameObject::MoveToFront()
	{
		if (GameObject* parent = GetParent())
		{
			auto move_to_front_action = [this, parent]()
			{
				auto list = &(parent->_objects);
				auto it = std::find(list->begin(), list->end(), this);

				assert(*it == this);

				auto tmp = *it;
				it = list->erase(it);
				list->push_back(tmp);
			};

			_preupdate_actions.push_back(move_to_front_action);
		}
	}

	void GameObject::MoveUnderTo(GameObject* obj)
	{
		if (GameObject* parent = GetParent())
		{
			auto move_under_action = [this, parent, obj]()
			{
				auto list = &(parent->_objects);
				auto this_obj = std::find(list->begin(), list->end(), this);
				auto other_obj = std::find(list->begin(), list->end(), obj);

				assert(this_obj != list->end() && other_obj != list->end());

				list->erase(this_obj);
				list->insert(other_obj, this);
			};

			_preupdate_actions.push_back(move_under_action);
		}
	}

	void GameObject::ForeachObject(std::function<void(GameObject*)> predicate)
	{
		for (auto& obj : _objects)
			predicate(obj);
	}

	void GameObject::ForeachObject(std::function<void(GameObject*, bool& need_break)> predicate)
	{
		bool need_break = false;
		for (auto& obj : _objects)
		{
			predicate(obj, need_break);
			if (need_break)
				break;
		}
	}

	void GameObject::RemoveObject(GameObject* obj)
	{
		auto action = [this, obj]()
		{
			auto it = std::find(_objects.begin(), _objects.end(), obj);

			assert(it != _objects.end());

			_objects.erase(it);
			delete obj;
		};

		_preupdate_actions.push_back(action);
	}

	void GameObject::Clear()
	{
		for (auto obj : _objects)
			delete obj;
		_objects.clear();
	}

	std::vector<std::function<void()>> GameObject::_preupdate_actions = std::vector<std::function<void()>>();

	void GameObject::InvokePreupdateActions()
	{
		for (auto& action : _preupdate_actions)
			action();
		_preupdate_actions.clear();
	}

	void GameObject::Update(int delta_time)
	{
		if (IsEnabled())
		{
			for (auto& obj : _objects)
				if (!obj->_started)
				{
					obj->_started = true;
					obj->Init();
				}

			for (auto& obj : _objects)
				if (obj->IsEnabled())
					obj->Update(delta_time);
		}
	}

	void GameObject::Enable()
	{
		_enable = true;
	}

	void GameObject::Disable()
	{
		_enable = false;
	}

	bool GameObject::IsEnabled() const
	{
		return _enable;
	}

	void GameObject::Hide()
	{
		_visible = false;
	}

	void GameObject::Show()
	{
		_visible = true;
	}

	bool GameObject::IsVisible() const
	{
		return _visible;
	}

	void GameObject::TurnOn()
	{
		Show();
		Enable();
	}

	void GameObject::TurnOff()
	{
		Hide();
		Disable();
	}

	void GameObject::Draw(sf::RenderWindow* window)
	{
		if (IsVisible())
			for (auto& obj : _objects)
				if (obj->IsVisible())
					obj->Draw(window);
	}

	void GameObject::PostDraw(sf::RenderWindow* window)
	{
		if (IsVisible())
			for (auto& obj : _objects)
				if (obj->IsVisible())
					obj->PostDraw(window);
	}

	const Vector& GameObject::GetPosition() const
	{
		return _pos;
	}

	void GameObject::SetPosition(const Vector& point)
	{
		OnPositionChanged(point, _pos);
		_pos = point;
	}

	void GameObject::SetPosition(float x, float y)
	{
		SetPosition(Vector(x, y));
	}

	void GameObject::Move(const Vector& point)
	{
		_pos += point;
	}

	Rect GameObject::GetBounds() const
	{
		return Rect(_pos, _size);
	}

	void GameObject::SetBounds(const Rect& rect)
	{
		_pos = rect.LeftTop;
		_size = rect.Size;
	}

	void GameObject::SetSize(const Vector& size)
	{
		_size = size;
	}

	void GameObject::OnPropertySet(const std::string& name)
	{
		if (name == "X")
			SetPosition(_properties["X"].AsFloat(), _pos.Y);
		else if (name == "Y")
			SetPosition(_pos.X, _properties["Y"].AsFloat());
		else if (name == "Name")
			SetName(_properties["Name"].AsString());
	}
}