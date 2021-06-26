#pragma once

class Animator;
class Rigidbody;
class ITransform;

#include "IUpdatable.h"

class Entity : public ITransform {
public:  
  inline static std::vector<IUpdatable*> updatableComponents;

  inline static void UpdateComponents() {
    for (auto& component : updatableComponents) {
      component->Update();
    }
  }

  inline Entity() {
    static unsigned int ids = 0;

    id = ids++;
  }

  inline unsigned int GetID() const {
    return id;
  }

  Animator* animator;
  Rigidbody* rigidbody;
  std::map<std::string, Ref<Component>> components;

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
    components[prettyName] = r;

    if (prettyName == "Animator") {
      animator = dynamic_cast<Animator*>(r.get());
    }

    if (prettyName == "Rigidbody") {
      rigidbody = dynamic_cast<Rigidbody*>(r.get());
    }

    if (IUpdatable* updatable = dynamic_cast<IUpdatable*>(r.get())) {
      updatableComponents.push_back(updatable);
    }

    return r.get();
  }  

  template <typename T>
  T* GetComponent() {
    std::string prettyName = NATURAFORGE_CLASS_NAME(T);
    return dynamic_cast<T*>(components[prettyName].get());
  }

private:
  unsigned int id;
};