# Cyros365 Unreal Assignment

## Overview
- The project fetches JSON data containing box types and objects.
- Parses data to spawn colored boxes with health and score properties.
- Shooting applies damage; boxes destroy when health reaches 0.

## Features
- JSON fetched at runtime via HTTP.
- Dynamic spawning with transforms from JSON.
- Dynamic material color applied to boxes.
- C++ driven functionality with Unreal standards.

## Challenges
- JSON parsing required attention to data types.
- Material setup for dynamic color needed blueprint-material workflow.

## How to Test
1. Open the project in Unreal 5.
2. Place `BoxSpawner` in the level.
3. Assign `BoxActor` Blueprint as Box Actor Class in `BoxSpawner`.
4. Play the game, observe boxes spawning based on JSON.
5. Use shooting to apply damage and destroy boxes.

 
