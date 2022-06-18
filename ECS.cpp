#pragma once
#include "headers/Component.h"
#include <stdio.h>
#include <cassert>
#include <string.h>

#ifndef KAMSKI_MAX_ENTITY_COUNT
#define KAMSKI_MAX_ENTITY_COUNT 10000
#endif

using EntityId = u32;

struct Entity
{
	EntityId id;
	u32 eraseCount;
    
    bool operator==(const Entity& other) const
    {
        return id == other.id && eraseCount == other.eraseCount;
    }
};


class IDStack
{
	public:
    
    IDStack()
    {
        memset(this, 0,sizeof(IDStack));
    }
    
    void push(Entity entity)
    {
        assert(top != KAMSKI_MAX_ENTITY_COUNT);
        ids[top] = entity;
        top++;
    }
    
    void pop()
    {
        assert(top != 0);
        top--;
    }
    
    bool empty() const
    {
        return top == 0;
    }
    
    Entity front() const
    {
        assert(!empty());
        return ids[top-1];
    }
    
	private:
    u32 top;
    Entity ids[KAMSKI_MAX_ENTITY_COUNT];
};


class EntityView
{
	public:
    EntityView(EntityId* const begin, EntityId* const end) :
    b(begin), e(end)
	{
	}
    
    EntityId* begin()
    {
        return b;
    }
    
    EntityId* end()
    {
        return e;
    }
    
    const EntityId* begin() const
    {
        return b;
    }
    
    const EntityId* end() const
    {
        return e;
    }
    
	private:
    EntityId* b;
    EntityId* e;
};


/// ################################
/// ##### ComponentView Section ####
/// ################################


class TransformComponentView
{
    public:
    TransformComponentView(TransformComponent* begin, TransformComponent* end)
    {
        b = begin;
        e = end;
    }
            
    TransformComponent* begin()
    {
        return b;
    }
            
    TransformComponent* end()
    {
        return e;
    }
            
    const TransformComponent* begin() const
    {
        return b;
    }
            
    const TransformComponent* end() const
    {
        return e;
    }
            
    private:
    TransformComponent* b;
    TransformComponent* e;
};

class TypeComponentView
{
    public:
    TypeComponentView(TypeComponent* begin, TypeComponent* end)
    {
        b = begin;
        e = end;
    }
            
    TypeComponent* begin()
    {
        return b;
    }
            
    TypeComponent* end()
    {
        return e;
    }
            
    const TypeComponent* begin() const
    {
        return b;
    }
            
    const TypeComponent* end() const
    {
        return e;
    }
            
    private:
    TypeComponent* b;
    TypeComponent* e;
};

class ColliderComponentView
{
    public:
    ColliderComponentView(ColliderComponent* begin, ColliderComponent* end)
    {
        b = begin;
        e = end;
    }
            
    ColliderComponent* begin()
    {
        return b;
    }
            
    ColliderComponent* end()
    {
        return e;
    }
            
    const ColliderComponent* begin() const
    {
        return b;
    }
            
    const ColliderComponent* end() const
    {
        return e;
    }
            
    private:
    ColliderComponent* b;
    ColliderComponent* e;
};

class SpriteComponentView
{
    public:
    SpriteComponentView(SpriteComponent* begin, SpriteComponent* end)
    {
        b = begin;
        e = end;
    }
            
    SpriteComponent* begin()
    {
        return b;
    }
            
    SpriteComponent* end()
    {
        return e;
    }
            
    const SpriteComponent* begin() const
    {
        return b;
    }
            
    const SpriteComponent* end() const
    {
        return e;
    }
            
    private:
    SpriteComponent* b;
    SpriteComponent* e;
};

class SolidColorComponentView
{
    public:
    SolidColorComponentView(SolidColorComponent* begin, SolidColorComponent* end)
    {
        b = begin;
        e = end;
    }
            
    SolidColorComponent* begin()
    {
        return b;
    }
            
    SolidColorComponent* end()
    {
        return e;
    }
            
    const SolidColorComponent* begin() const
    {
        return b;
    }
            
    const SolidColorComponent* end() const
    {
        return e;
    }
            
    private:
    SolidColorComponent* b;
    SolidColorComponent* e;
};

class EnemyComponentView
{
    public:
    EnemyComponentView(EnemyComponent* begin, EnemyComponent* end)
    {
        b = begin;
        e = end;
    }
            
    EnemyComponent* begin()
    {
        return b;
    }
            
    EnemyComponent* end()
    {
        return e;
    }
            
    const EnemyComponent* begin() const
    {
        return b;
    }
            
    const EnemyComponent* end() const
    {
        return e;
    }
            
    private:
    EnemyComponent* b;
    EnemyComponent* e;
};

class EffectComponentView
{
    public:
    EffectComponentView(EffectComponent* begin, EffectComponent* end)
    {
        b = begin;
        e = end;
    }
            
    EffectComponent* begin()
    {
        return b;
    }
            
    EffectComponent* end()
    {
        return e;
    }
            
    const EffectComponent* begin() const
    {
        return b;
    }
            
    const EffectComponent* end() const
    {
        return e;
    }
            
    private:
    EffectComponent* b;
    EffectComponent* e;
};

class VelocityComponentView
{
    public:
    VelocityComponentView(VelocityComponent* begin, VelocityComponent* end)
    {
        b = begin;
        e = end;
    }
            
    VelocityComponent* begin()
    {
        return b;
    }
            
    VelocityComponent* end()
    {
        return e;
    }
            
    const VelocityComponent* begin() const
    {
        return b;
    }
            
    const VelocityComponent* end() const
    {
        return e;
    }
            
    private:
    VelocityComponent* b;
    VelocityComponent* e;
};

class FollowComponentView
{
    public:
    FollowComponentView(FollowComponent* begin, FollowComponent* end)
    {
        b = begin;
        e = end;
    }
            
    FollowComponent* begin()
    {
        return b;
    }
            
    FollowComponent* end()
    {
        return e;
    }
            
    const FollowComponent* begin() const
    {
        return b;
    }
            
    const FollowComponent* end() const
    {
        return e;
    }
            
    private:
    FollowComponent* b;
    FollowComponent* e;
};

class ProjectileComponentView
{
    public:
    ProjectileComponentView(ProjectileComponent* begin, ProjectileComponent* end)
    {
        b = begin;
        e = end;
    }
            
    ProjectileComponent* begin()
    {
        return b;
    }
            
    ProjectileComponent* end()
    {
        return e;
    }
            
    const ProjectileComponent* begin() const
    {
        return b;
    }
            
    const ProjectileComponent* end() const
    {
        return e;
    }
            
    private:
    ProjectileComponent* b;
    ProjectileComponent* e;
};

class HealthBarComponentView
{
    public:
    HealthBarComponentView(HealthBarComponent* begin, HealthBarComponent* end)
    {
        b = begin;
        e = end;
    }
            
    HealthBarComponent* begin()
    {
        return b;
    }
            
    HealthBarComponent* end()
    {
        return e;
    }
            
    const HealthBarComponent* begin() const
    {
        return b;
    }
            
    const HealthBarComponent* end() const
    {
        return e;
    }
            
    private:
    HealthBarComponent* b;
    HealthBarComponent* e;
};

class ItemComponentView
{
    public:
    ItemComponentView(ItemComponent* begin, ItemComponent* end)
    {
        b = begin;
        e = end;
    }
            
    ItemComponent* begin()
    {
        return b;
    }
            
    ItemComponent* end()
    {
        return e;
    }
            
    const ItemComponent* begin() const
    {
        return b;
    }
            
    const ItemComponent* end() const
    {
        return e;
    }
            
    private:
    ItemComponent* b;
    ItemComponent* e;
};


/// ##################################
/// ##### ComponentVector Section ####
/// ##################################


class TransformComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    TransformComponent& addComponent(const EntityId entity, TransformComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    TransformComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const TransformComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    TransformComponentView iterateComponents()
    {
        return TransformComponentView((TransformComponent*)compArray, (TransformComponent*)compArray + denseSize);
    }
            
    const TransformComponentView iterateComponents() const
    {
        return TransformComponentView((TransformComponent*)compArray, (TransformComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    TransformComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class TypeComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    TypeComponent& addComponent(const EntityId entity, TypeComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    TypeComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const TypeComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    TypeComponentView iterateComponents()
    {
        return TypeComponentView((TypeComponent*)compArray, (TypeComponent*)compArray + denseSize);
    }
            
    const TypeComponentView iterateComponents() const
    {
        return TypeComponentView((TypeComponent*)compArray, (TypeComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    TypeComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class ColliderComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    ColliderComponent& addComponent(const EntityId entity, ColliderComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    ColliderComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const ColliderComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    ColliderComponentView iterateComponents()
    {
        return ColliderComponentView((ColliderComponent*)compArray, (ColliderComponent*)compArray + denseSize);
    }
            
    const ColliderComponentView iterateComponents() const
    {
        return ColliderComponentView((ColliderComponent*)compArray, (ColliderComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    ColliderComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class SpriteComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    SpriteComponent& addComponent(const EntityId entity, SpriteComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    SpriteComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const SpriteComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    SpriteComponentView iterateComponents()
    {
        return SpriteComponentView((SpriteComponent*)compArray, (SpriteComponent*)compArray + denseSize);
    }
            
    const SpriteComponentView iterateComponents() const
    {
        return SpriteComponentView((SpriteComponent*)compArray, (SpriteComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    SpriteComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class SolidColorComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    SolidColorComponent& addComponent(const EntityId entity, SolidColorComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    SolidColorComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const SolidColorComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    SolidColorComponentView iterateComponents()
    {
        return SolidColorComponentView((SolidColorComponent*)compArray, (SolidColorComponent*)compArray + denseSize);
    }
            
    const SolidColorComponentView iterateComponents() const
    {
        return SolidColorComponentView((SolidColorComponent*)compArray, (SolidColorComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    SolidColorComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class EnemyComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    EnemyComponent& addComponent(const EntityId entity, EnemyComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    EnemyComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const EnemyComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    EnemyComponentView iterateComponents()
    {
        return EnemyComponentView((EnemyComponent*)compArray, (EnemyComponent*)compArray + denseSize);
    }
            
    const EnemyComponentView iterateComponents() const
    {
        return EnemyComponentView((EnemyComponent*)compArray, (EnemyComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    EnemyComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class EffectComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    EffectComponent& addComponent(const EntityId entity, EffectComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    EffectComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const EffectComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    EffectComponentView iterateComponents()
    {
        return EffectComponentView((EffectComponent*)compArray, (EffectComponent*)compArray + denseSize);
    }
            
    const EffectComponentView iterateComponents() const
    {
        return EffectComponentView((EffectComponent*)compArray, (EffectComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    EffectComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class VelocityComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    VelocityComponent& addComponent(const EntityId entity, VelocityComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    VelocityComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const VelocityComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    VelocityComponentView iterateComponents()
    {
        return VelocityComponentView((VelocityComponent*)compArray, (VelocityComponent*)compArray + denseSize);
    }
            
    const VelocityComponentView iterateComponents() const
    {
        return VelocityComponentView((VelocityComponent*)compArray, (VelocityComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    VelocityComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class FollowComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    FollowComponent& addComponent(const EntityId entity, FollowComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    FollowComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const FollowComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    FollowComponentView iterateComponents()
    {
        return FollowComponentView((FollowComponent*)compArray, (FollowComponent*)compArray + denseSize);
    }
            
    const FollowComponentView iterateComponents() const
    {
        return FollowComponentView((FollowComponent*)compArray, (FollowComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    FollowComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class ProjectileComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    ProjectileComponent& addComponent(const EntityId entity, ProjectileComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    ProjectileComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const ProjectileComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    ProjectileComponentView iterateComponents()
    {
        return ProjectileComponentView((ProjectileComponent*)compArray, (ProjectileComponent*)compArray + denseSize);
    }
            
    const ProjectileComponentView iterateComponents() const
    {
        return ProjectileComponentView((ProjectileComponent*)compArray, (ProjectileComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    ProjectileComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class HealthBarComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    HealthBarComponent& addComponent(const EntityId entity, HealthBarComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    HealthBarComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const HealthBarComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    HealthBarComponentView iterateComponents()
    {
        return HealthBarComponentView((HealthBarComponent*)compArray, (HealthBarComponent*)compArray + denseSize);
    }
            
    const HealthBarComponentView iterateComponents() const
    {
        return HealthBarComponentView((HealthBarComponent*)compArray, (HealthBarComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    HealthBarComponent compArray[denseCapacity];
            
    u32 denseSize;
};

class ItemComponentVector
{
    public:
            
    void clear()
    {
        denseSize = 0;
    }
            
    ItemComponent& addComponent(const EntityId entity, ItemComponent& c)
    {
        assert(entity < sparseSize);
        assert(denseSize != denseCapacity);
                    
        sparse[entity] = denseSize;
        dense[denseSize] = entity;
        compArray[denseSize] = c;
        return compArray[denseSize++];
    }
            
    bool hasComponent(const EntityId entity) const
    {
        return !(entity >= sparseSize || denseSize <= sparse[entity] || dense[sparse[entity]] != entity);
    }
            
    void removeComponent(EntityId entity)
    {
        if (!hasComponent(entity))
        {
            return;
        }
                    
        const u32 toRemoveIndex = sparse[entity];
        const u32 lastIndex = denseSize - 1;
                    
        EntityId lastEntity = dense[lastIndex];
        dense[toRemoveIndex] = lastEntity;
        sparse[lastEntity] = toRemoveIndex;
        compArray[toRemoveIndex] = compArray[lastIndex];
        denseSize--;
    }
            
    // Crashes if entity[entity] doesn't have this Component
    ItemComponent& getComponent(const Entity entity)
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Crashes if entity[entity] doesn't have this Component
    const ItemComponent& getComponent(const Entity entity) const
    {
        assert(hasComponent(entity));
        return compArray[sparse[entity]];
    }
            
    // Ranged for functions
            
    ItemComponentView iterateComponents()
    {
        return ItemComponentView((ItemComponent*)compArray, (ItemComponent*)compArray + denseSize);
    }
            
    const ItemComponentView iterateComponents() const
    {
        return ItemComponentView((ItemComponent*)compArray, (ItemComponent*)compArray + denseSize);
    }
            
    EntityView iterateEntities()
    {
        return EntityView{ (EntityId*)dense, (EntityId*)dense + denseSize };
    }
            
    const EntityView iterateEntities() const
    {
        return EntityView((EntityId*)dense, (EntityId*)dense + denseSize);
    }
            
    u64 size() const
    {
        return denseSize;
    }
            
    private:
            
    static constexpr u32 sparseSize = KAMSKI_MAX_ENTITY_COUNT;
    static constexpr u32 denseCapacity = KAMSKI_MAX_ENTITY_COUNT;
            
    u32 sparse[sparseSize];
    EntityId dense[denseCapacity];
    ItemComponent compArray[denseCapacity];
            
    u32 denseSize;
};


enum SignatureTag
{
	
	SIGNATURE_TAG_COUNT
};

struct Signature
{
    static constexpr u64 ACTUAL_BIT_COUNT = (u64)SIGNATURE_TAG_COUNT / 64ull + (u64)(SIGNATURE_TAG_COUNT % 64 != 0); 
	u64 bytes[ACTUAL_BIT_COUNT];
    
    constexpr bool operator==(const Signature& other) const
    {
        for (u64 i = 0; i < ACTUAL_BIT_COUNT; i++)
        {
            if (other.bytes[i] != bytes[i])
                return false;
        }
        return true;
    }
    
    constexpr bool operator!=(const Signature& other) const
    {
        for (u64 i = 0; i < ACTUAL_BIT_COUNT; i++)
        {
            if (other.bytes[i] != bytes[i])
                return true;
        }
        return false;
    }
    
    constexpr Signature operator|(const Signature& other) const
    {
        Signature retval = {};
        for (u64 i = 0; i < ACTUAL_BIT_COUNT; i++)
        {
            retval.bytes[i] = bytes[i] | other.bytes[i];
        }
        return retval;
    }
    
    constexpr Signature operator&(const Signature& other) const
    {
        Signature retval = {};
        for (u64 i = 0; i < ACTUAL_BIT_COUNT; i++)
        {
            retval.bytes[i] = bytes[i] & other.bytes[i];
        }
        return retval;
    }
    
};

template<typename Sig, typename _ComponentList>
class SignatureIterator
{
    public:
    
    SignatureIterator(Signature* ptr, Signature* end, Signature toFind):
    ptr(ptr),
    end(end),
    toFind(toFind)
    {
    }
    
    Entity operator*() const
    {
        return ptr->eId;
    }
    
    SignatureIterator& operator++()
    {
        ptr++;
        while (ptr != end && ((*ptr) & toFind) != toFind)
        {
            ptr++;
        }
        return *this;
    }
    
    SignatureIterator operator++(int)
    {
        SignatureIterator temp = *this;
        ptr++;
        while (ptr != end && ((*ptr) & toFind) != toFind)
        {
            ptr++;
        }
        return temp;
    }
    
    bool operator==(const SignatureIterator& other) const
    {
        return ptr == other.ptr;
    }
    
    bool operator!=(const SignatureIterator& other) const
    {
        return ptr != other.ptr;
    }
    
    private:
    Signature* ptr;
    const Signature* end;
    Signature toFind;
};


template<typename Sig, typename _ComponentList>
class ConstSignatureIterator
{
    public:
    
    ConstSignatureIterator(const Signature* ptr, const Signature* end, Signature toFind):
    ptr(ptr),
    end(end),
    toFind(toFind)
    {
    }
    
    Entity operator*() const
    {
        return ptr->eId;
    }
    
    ConstSignatureIterator& operator++()
    {
        ptr++;
        while (ptr != end && ((*ptr) & toFind) != toFind)
        {
            ptr++;
        }
        return *this;
    }
    
    ConstSignatureIterator operator++(int)
    {
        ConstSignatureIterator temp = *this;
        ptr++;
        while (ptr != end && ((*ptr) & toFind) != toFind)
        {
            ptr++;
        }
        return temp;
    }
    
    bool operator==(const ConstSignatureIterator& other) const
    {
        return ptr == other.ptr;
    }
    
    bool operator!=(const ConstSignatureIterator& other) const
    {
        return ptr != other.ptr;
    }
    
    private:
    const Signature* ptr;
    const Signature* end;
    Signature toFind;
    
};

class SignatureView
{
    public:
    SignatureView(Signature* _begin, Signature* _end, Signature toFind):
    _end(_end),
    toFind(toFind)
    {
        
        while (_begin != _end)
        {
            Signature res = ((*_begin) & toFind);
            if (res != toFind)
                _begin++;
            else
                break;
        }
        this->_begin = _begin;
    }
    
    SignatureIterator begin()
    {
        SignatureIterator ret(_begin,_end);
        return ret;
    }
    
    SignatureIterator end()
    {
        SignatureIterator ret(_end,_end);
        return ret;
    }
    
    ConstSignatureIterator begin() const
    {
        ConstSignatureIterator ret(_begin,_end);
        return ret;
    }
    
    ConstSignatureIterator end() const
    {
        ConstSignatureIterator ret(_end,_end);
        return ret;
    }
    
    private:
    Signature* _begin;
    Signature* _end;
    Signature toFind;
};

class EntityRegistry
{
	public:
    
    EntityRegistry():
    nextEntity(0)
	{
        
	}
    
    bool isMarked(EntityId eId)
    {
        for (u32 i = entityCount - markedBegin; i != entityCount; i++)
        {
            if(entities[i].id == eId)
                return false;
        }
        return true;
    }
    
    void removeMarkedEntities()
    {
        for (u32 i = entityCount - markedBegin; i != entityCount; i++)
        {
            removeEntity(entities[i].entity);
        }
        entityCount -= markedBegin;
        markedBegin = 0;
    }
    
    Entity createEntity()
    {
        Entity retval = nextEntity;
        if (!previousIds.empty())
        {
            retval = previousIds.front();
            previousIds.pop();
        }
        else
        {
            assert(nextEntity != KAMSKI_MAX_ENTITY_COUNT);
            nextEntity.id++;
        }
        assert(entityCount != entityCapacity);
        entityIndices[retval.id] = entityCount;
        entitySignatures[entityCount] = {};
        entities[entityCount] = retval;
        entityCount++;
        return retval;
    }
    
    void markEntityForDeletion(EntityId eId)
    {
        if (markedBegin == entityCount || entityIndices[eId] > entityCount - 1 - markedBegin)
        {
            return;
        }
        const u32 lastIndex = entityCount - 1 - markedBegin;
        
        if (lastIndex != entityIndices[eId])
        {
            Signature aux = entitySignatures[lastIndex];
            entitySignatures[lastIndex] = entitySignatures[entityIndices[eId]];
            entitySignatures[entityIndices[eId]] = aux;
            
            Entity eAux = entities[lastIndex];
            entities[lastIndex] = entities[entityIndices[eId]];
            entities[entityIndices[eId]] = eAux;
            
            entityIndices[eAux.id] = entityIndices[eId];
            entityIndices[eId] = lastIndex;
        }
        markedBegin++;
    }
    
    //TODO: maybe check if entity exists
    void removeEntity(EntityId eId)
    {
        
        cvTransformComponent.removeComponent(eId);
        
        cvTypeComponent.removeComponent(eId);
        
        cvColliderComponent.removeComponent(eId);
        
        cvSpriteComponent.removeComponent(eId);
        
        cvSolidColorComponent.removeComponent(eId);
        
        cvEnemyComponent.removeComponent(eId);
        
        cvEffectComponent.removeComponent(eId);
        
        cvVelocityComponent.removeComponent(eId);
        
        cvFollowComponent.removeComponent(eId);
        
        cvProjectileComponent.removeComponent(eId);
        
        cvHealthBarComponent.removeComponent(eId);
        
        cvItemComponent.removeComponent(eId);
        
        
        u32 eraseCount = entities[entityIndices[eId]].eraseCount;
        previousIds.push({eId, eraseCount});
    }
    
    bool entityExists(Entity entity) const
    {
        return entityIndices[entity.id] < entityCount && entities[entityIndices[entity.id]].eraseCount == entity.eraseCount;
    }
    
    SignatureView iterateEntities(Signature sig)
    {
        SignatureView retval(&entitySignatures[0], &entitySignatures[entityCount], sig);
        return retval;
    }
    
    const SignatureView iterateEntities(Signature sig) const
    {
        const SignatureView retval(&entitySignatures[0], &entitySignatures[entityCount], sig);
        return retval;
    }
    
    
    TransformComponentVector cvTransformComponent;
    
    TypeComponentVector cvTypeComponent;
    
    ColliderComponentVector cvColliderComponent;
    
    SpriteComponentVector cvSpriteComponent;
    
    SolidColorComponentVector cvSolidColorComponent;
    
    EnemyComponentVector cvEnemyComponent;
    
    EffectComponentVector cvEffectComponent;
    
    VelocityComponentVector cvVelocityComponent;
    
    FollowComponentVector cvFollowComponent;
    
    ProjectileComponentVector cvProjectileComponent;
    
    HealthBarComponentVector cvHealthBarComponent;
    
    ItemComponentVector cvItemComponent;
    
    
	private:
    
    IDStack previousIds;
    static constexpr u32 entityCapacity = KAMSKI_MAX_ENTITY_COUNT;
    u32 entityIndices[entityCapacity];
    Signature entitySignatures[entityCapacity];
    Entity entities[entityCapacity];
    u32 entityCount;
    u32 markedBegin;
    
    Entity nextEntity;
};