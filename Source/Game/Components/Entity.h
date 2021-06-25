#pragma once

class Animator;
class Rigidbody;
class ITransform;

class Entity : public ITransform {
public:  
  Ref<Animator> animator;
  Ref<Rigidbody> rigidbody;
  std::map<std::string, Component*> components;

  template <typename T, typename... Args>
  T* AddComponent(Args... args) {
    #ifdef NATURAFORGE_COMPILER_VS
      std::string rawName = typeid(T).name();
      std::string prettyName = rawName.substr(rawName.find_first_of(' ') + 1, rawName.size());
    #else
      std::string rawName = typeid(T).name();
      std::string prettyName = rawName.substr(rawName.find_first_of(' ') + 2, rawName.size());
    #endif

    Ref<T> r = CreateRef<T>(this, std::forward<Args>(args)...);
    components[prettyName] = r.get();

    if (prettyName == "Animator") {
      animator = std::dynamic_pointer_cast<Animator>(r);
    }

    if (prettyName == "Rigidbody") {
      rigidbody = std::dynamic_pointer_cast<Rigidbody>(r);
    }

    return static_cast<T*>(components[prettyName]);
  }  

  template <typename T>
  T* GetComponent() {
    std::string prettyName = NATURAFORGE_CLASS_NAME(T);
    return static_cast<T*>(components[prettyName]);
  }
};