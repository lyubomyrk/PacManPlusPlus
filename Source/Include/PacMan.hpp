/*
 * Created on Tue Oct 17 2023
 *
 * Copyright (C) 2023 Lyubomyr Kryshtanovskyi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "Globals.hpp"
#include "Entity.hpp"
#include "MovingEntity.hpp"
#include "MovementComponent.hpp"
#include "DirectionComponent.hpp"
#include "Timer.hpp"

class PacMan : public MovingEntity
{
    Texture2D _spritesheet;
    Color _tint;
    int _frameTicks;
    int _frameRate;
    int _frame;
    int _maxFrame;

    Vector2 _position;
    Vector2 _normDir;
    Vector2 _normDirBuffer;
    DirectionComponent *_directionComponent;

    float _speed;
    MovementComponent *_movementComponent;

    bool _alive;

    Timer _wakaTimer;
    const float _wakaPlaytime = (1. / TargetFps) * (1. / PacmanSpeed) * (TileUnit);

    Timer _deathTimer;
    const float _deathPlaytime = ((float)PacmanDeathFrames / PacmanDeathFps);

    Timer _energizedTimer;
    bool _energized;

public:
    PacMan(DirectionComponent *directionComponent, MovementComponent *movementComponent);
    ~PacMan() override;

    void HandleInput();
    void Update();
    void Draw();

    void Start();
    void Kill();
    bool IsAlive() const;
    bool IsDeathFinished();
    bool IsEnergized() const;
    void AtePellet();
    void Energize();
    void Reset();

    Vector2 GetPosition() const override;
    void SetPosition(Vector2 position) override;
    Vector2 GetDirection() const override;
    void SetDirection(Vector2 direction) override;
    Vector2 GetDirectionBuffer() const override;
    float GetSpeed() const override;
    void SetSpeed(float speed) override;
    Rectangle GetRectangle() const override;

private:
};