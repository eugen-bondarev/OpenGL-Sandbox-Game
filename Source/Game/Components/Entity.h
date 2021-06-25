#pragma once

class Animator;
class Rigidbody;
class ITransform;

class Entity : public ITransform {
public:  
  Ref<Animator> animator;
  Ref<Rigidbody> rigidbody;
  std::map<std::string, Ref<Component>> components;

  template <typename T, typename... Args>
  T* AddComponent(Args... args) {
		std::string rawName = typeid(T).name();
		std::string prettyName = rawName.substr(rawName.find_first_of(' ') + 1, rawName.size());
    components[prettyName] = CreateRef<T>(Ref<Entity>(this), std::forward<Args>(args)...);

    if (prettyName == "Animator") {
      animator = std::dynamic_pointer_cast<Animator>(components[prettyName]);
    }

    if (prettyName == "Rigidbody") {
      rigidbody = std::dynamic_pointer_cast<Rigidbody>(components[prettyName]);
    }

    return static_cast<T*>(components[prettyName].get());
  }  

  template <typename T>
  T* GetComponent() {
		std::string rawName = typeid(T).name();
		std::string prettyName = rawName.substr(rawName.find_first_of(' ') + 1, rawName.size());

    return static_cast<T*>(components[prettyName].get());
  }
};