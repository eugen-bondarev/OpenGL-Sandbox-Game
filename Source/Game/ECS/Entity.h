#pragma once

#include "IUpdatable.h"

class Animator;
class Rigidbody;
class ITransform;

class Entity : public ITransform {
public:
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
    std::string prettyName = NF_CLASS_NAME(T);

    Ref<T> r = CreateRef<T>(this, std::forward<Args>(args)...);
    components[prettyName] = r;

    if (prettyName == "Animator") {
      animator = dynamic_cast<Animator*>(r.get());
    }

    if (prettyName == "Rigidbody") {
      rigidbody = dynamic_cast<Rigidbody*>(r.get());
    }

    if (IUpdatable* updatable = dynamic_cast<IUpdatable*>(r.get())) {
      IUpdatable::updatableComponents.push_back(updatable);
    }

    return r.get();
  }

  template <typename T>
  void DeleteComponent() {
    std::string prettyName = NF_CLASS_NAME(T);
    
    components.erase(components.find(prettyName));
  }

  template <typename T>
  T* GetComponent() {
    std::string prettyName = NF_CLASS_NAME(T);

    return dynamic_cast<T*>(components[prettyName].get());
  }

private:
  unsigned int id;
};