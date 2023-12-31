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

#include "PacMan.hpp"
#include "AssetManager.hpp"

PacMan::PacMan(DirectionComponent *directionComponent, MovementComponent *movementComponent)
{
    _spritesheet = AssetManager::TPacMan32;
    _tint = WHITE;
    _frameRate = PacmanMovingFps;
    _maxFrame = PacmanMovingFrames;
    _frameTicks = 0;
    _frame = 0;

    _position = PacmanStartingPosition;
    _normDir = Direction::Left();
    _normDirBuffer = _normDir;
    _directionComponent = directionComponent;
    _speed = 0.;
    _movementComponent = movementComponent;

    _alive = true;
}

PacMan::~PacMan()
{
}

void PacMan::Reset()
{
    _spritesheet = AssetManager::TPacMan32;
    _tint = WHITE;
    _frameRate = PacmanMovingFps;
    _maxFrame = PacmanMovingFrames;
    _frameTicks = 0;
    _frame = 0;

    _position = PacmanStartingPosition;
    _normDir = Direction::Left();
    _normDirBuffer = _normDir;

    _speed = 0.;

    _alive = true;
    _energized = false;
}

void PacMan::HandleInput()
{
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
    {
        _normDirBuffer = Direction::Left();
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
    {
        _normDirBuffer = Direction::Right();
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        _normDirBuffer = Direction::Up();
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        _normDirBuffer = Direction::Down();
    }
}

void PacMan::Update()
{
    _directionComponent->Update(this);
    _movementComponent->Update(this);

    _wakaTimer.Update();
    _deathTimer.Update();
    _energizedTimer.Update();

    if (_energized && _energizedTimer.IsFinished())
    {
        _energized = false;
    }

    PlaySoundIfTrue(AssetManager::SWaka, !_wakaTimer.IsFinished());
    PlaySoundIfTrue(AssetManager::SDeath, !_alive && !_deathTimer.IsFinished());

    if (_frameTicks++ >= (TargetFps / _frameRate))
    {
        _frameTicks = 0;
        if (_frame++ >= _maxFrame)
        {
            _frame = 0;
        }
    }
}

void PacMan::Draw()
{
    float rotation = 0.;

    if (_normDir == Direction::Right())
    {
        rotation = 0.;
    }
    else if (_normDir == Direction::Down())
    {
        rotation = 90.;
    }
    else if (_normDir == Direction::Left())
    {
        rotation = 180.;
    }
    else if (_normDir == Direction::Up())
    {
        rotation = 270.;
    }

    DrawTexturePro(
        _spritesheet,
        {(float)_frame * SpriteUnit, 0, SpriteUnit, SpriteUnit},
        {_position.x, _position.y, SpriteUnit, SpriteUnit},
        {SpriteUnitOffset, SpriteUnitOffset},
        rotation,
        _tint);
}

void PacMan::Start()
{
    _speed = PacmanSpeed;
}

void PacMan::Kill()
{
    if (_alive)
    {
        _alive = false;
        _speed = 0.;
        _deathTimer.Start(_deathPlaytime);
        _spritesheet = AssetManager::TPacManDeath32;
        _maxFrame = PacmanDeathFrames;
        _frameRate = PacmanDeathFps;
        _frame = 0;
        _frameTicks = 0;
    }
}

bool PacMan::IsAlive() const
{
    return _alive;
}

bool PacMan::IsDeathFinished()
{
    return !_alive && _deathTimer.IsFinished();
}

bool PacMan::IsEnergized() const
{
    return _energized;
}

void PacMan::AtePellet()
{
    _wakaTimer.Start(_wakaPlaytime);
}

void PacMan::Energize()
{
    _energizedTimer.Start(EnergizerTime);
    _energized = true;
}

Vector2 PacMan::GetPosition() const
{
    return _position;
}

void PacMan::SetPosition(Vector2 position)
{
    _position = position;
}

Vector2 PacMan::GetDirection() const
{
    return _normDir;
}

void PacMan::SetDirection(Vector2 direction)
{
    _normDir = direction;
}

Vector2 PacMan::GetDirectionBuffer() const
{
    return _normDirBuffer;
}

float PacMan::GetSpeed() const
{
    return _speed;
}

void PacMan::SetSpeed(float speed)
{
    _speed = speed;
}

Rectangle PacMan::GetRectangle() const
{
    return {
        _position.x * TileUnit - TileUnitOffset, _position.y * TileUnit - TileUnitOffset,
        (float)TileUnit, (float)TileUnit};
}
