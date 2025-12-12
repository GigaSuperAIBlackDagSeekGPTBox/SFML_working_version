#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include "Bone.h"

// Структура для хранения состояния кости в ключевом кадре
struct BoneKeyframe {
    sf::Vector2f position;
    float rotation;  // В градусах
    float time;      // Время в секундах от начала анимации
};

// Класс для хранения анимации одной кости
class BoneAnimation {
private:
    std::vector<BoneKeyframe> _keyframes;
    std::string _boneName;

public:
    BoneAnimation(const std::string& boneName) : _boneName(boneName) {}

    void addKeyframe(float time, const sf::Vector2f& position, float rotation) {
        BoneKeyframe keyframe;
        keyframe.time = time;
        keyframe.position = position;
        keyframe.rotation = rotation;
        _keyframes.push_back(keyframe);
        
        // Сортируем по времени
        std::sort(_keyframes.begin(), _keyframes.end(), 
            [](const BoneKeyframe& a, const BoneKeyframe& b) {
                return a.time < b.time;
            });
    }

    // Получить интерполированное состояние кости в заданное время
    BoneKeyframe getStateAtTime(float time, bool looping = true) const {
        if (_keyframes.empty()) {
            BoneKeyframe empty;
            empty.position = {0.0f, 0.0f};
            empty.rotation = 0.0f;
            empty.time = 0.0f;
            return empty;
        }

        float maxTime = _keyframes.back().time;
        
        // Если анимация зациклена, берем время по модулю
        if (looping && maxTime > 0.0f) {
            time = std::fmod(time, maxTime);
            // Если после fmod время отрицательное (не должно быть, но на всякий случай)
            if (time < 0.0f) {
                time += maxTime;
            }
        }

        // Если время меньше первого ключевого кадра
        if (time <= _keyframes[0].time) {
            return _keyframes[0];
        }

        // Если время больше последнего ключевого кадра (и не зациклено)
        if (time >= _keyframes.back().time && !looping) {
            return _keyframes.back();
        }

        // Находим два ключевых кадра для интерполяции
        for (size_t i = 0; i < _keyframes.size() - 1; ++i) {
            if (time >= _keyframes[i].time && time <= _keyframes[i + 1].time) {
                const BoneKeyframe& kf1 = _keyframes[i];
                const BoneKeyframe& kf2 = _keyframes[i + 1];
                
                float t = (time - kf1.time) / (kf2.time - kf1.time);
                
                BoneKeyframe result;
                result.position.x = kf1.position.x + (kf2.position.x - kf1.position.x) * t;
                result.position.y = kf1.position.y + (kf2.position.y - kf1.position.y) * t;
                
                // Интерполяция угла (с учетом кратчайшего пути)
                float angleDiff = kf2.rotation - kf1.rotation;
                // Нормализуем угол к диапазону [-180, 180]
                while (angleDiff > 180.0f) angleDiff -= 360.0f;
                while (angleDiff < -180.0f) angleDiff += 360.0f;
                result.rotation = kf1.rotation + angleDiff * t;
                result.time = time;
                
                return result;
            }
        }

        return _keyframes.back();
    }

    const std::string& getBoneName() const { return _boneName; }
    float getDuration() const {
        return _keyframes.empty() ? 0.0f : _keyframes.back().time;
    }
};

// Класс для управления скелетом
class Skeleton {
private:
    std::map<std::string, Bone*> _bones;
    Bone* _rootBone;
    std::map<Bone*, std::vector<Bone*>> _children; // Дочерние кости для каждой кости

    void drawBoneRecursive(Bone* bone, sf::RenderTarget& target) {
        bone->draw(target);
        
        // Рисуем дочерние кости
        auto it = _children.find(bone);
        if (it != _children.end()) {
            for (Bone* child : it->second) {
                drawBoneRecursive(child, target);
            }
        }
    }

public:
    Skeleton() : _rootBone(nullptr) {}

    void addBone(const std::string& name, Bone* bone) {
        _bones[name] = bone;
        if (!_rootBone) {
            _rootBone = bone;
        }
        
        // Добавляем в иерархию дочерних костей
        if (bone->getParent() != nullptr) {
            // Находим родительскую кость по указателю
            for (auto& pair : _bones) {
                if (pair.second == bone->getParent()) {
                    _children[pair.second].push_back(bone);
                    break;
                }
            }
        }
    }

    Bone* getBone(const std::string& name) {
        auto it = _bones.find(name);
        return (it != _bones.end()) ? it->second : nullptr;
    }

    void draw(sf::RenderTarget& target) {
        if (_rootBone) {
            drawBoneRecursive(_rootBone, target);
        }
    }

    // Получить все кости
    const std::map<std::string, Bone*>& getAllBones() const {
        return _bones;
    }
};

// Класс для полной скелетной анимации
class SkeletalAnimation {
private:
    std::map<std::string, BoneAnimation> _boneAnimations;
    float _duration;
    std::string _name;
    bool _isLooping;

public:
    SkeletalAnimation(const std::string& name, bool looping = true) 
        : _name(name), _isLooping(looping), _duration(0.0f) {}

    void addBoneAnimation(const std::string& boneName, const BoneAnimation& animation) {
        _boneAnimations.insert_or_assign(boneName, animation);
        _duration = std::max(_duration, animation.getDuration());
    }

    void addKeyframe(const std::string& boneName, float time, 
                     const sf::Vector2f& position, float rotation) {
        auto it = _boneAnimations.find(boneName);
        if (it == _boneAnimations.end()) {
            _boneAnimations.emplace(boneName, BoneAnimation(boneName));
            it = _boneAnimations.find(boneName);
        }
        it->second.addKeyframe(time, position, rotation);
        if (time > _duration) {
            _duration = time;
        }
    }

    // Применить анимацию к скелету в заданное время
    void applyToSkeleton(Skeleton& skeleton, float time) const {
        for (auto& pair : _boneAnimations) {
            Bone* bone = skeleton.getBone(pair.first);
            if (bone) {
                BoneKeyframe state = pair.second.getStateAtTime(time, _isLooping);
                bone->setLocalPositionVector(state.position);
                bone->setRotationAngle(state.rotation);
            }
        }
    }

    float getDuration() const { return _duration; }
    const std::string& getName() const { return _name; }
    bool isLooping() const { return _isLooping; }
};

// Аниматор для проигрывания скелетных анимаций
class SkeletalAnimator {
private:
    sf::Clock _clock;
    SkeletalAnimation* _currentAnimation;
    Skeleton* _skeleton;
    float _playbackSpeed;
    bool _isPlaying;

public:
    SkeletalAnimator(Skeleton* skeleton) 
        : _skeleton(skeleton), _currentAnimation(nullptr), 
          _playbackSpeed(1.0f), _isPlaying(false) {}

    void playAnimation(SkeletalAnimation* animation) {
        _currentAnimation = animation;
        _clock.restart();
        _isPlaying = true;
    }

    void stop() {
        _isPlaying = false;
    }

    void pause() {
        _isPlaying = false;
    }

    void resume() {
        if (_currentAnimation) {
            _isPlaying = true;
        }
    }

    void setPlaybackSpeed(float speed) {
        _playbackSpeed = speed;
    }

    void update() {
        if (_isPlaying && _currentAnimation && _skeleton) {
            float elapsedTime = _clock.getElapsedTime().asSeconds() * _playbackSpeed;
            _currentAnimation->applyToSkeleton(*_skeleton, elapsedTime);
        }
    }

    void draw(sf::RenderTarget& target) {
        if (_skeleton) {
            _skeleton->draw(target);
        }
    }

    bool isPlaying() const { return _isPlaying; }
    SkeletalAnimation* getCurrentAnimation() const { return _currentAnimation; }
    float getCurrentTime() const {
        return _isPlaying ? _clock.getElapsedTime().asSeconds() * _playbackSpeed : 0.0f;
    }
};
