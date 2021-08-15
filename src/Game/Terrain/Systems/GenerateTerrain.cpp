#include "GenerateTerrain.h"

#include "Game/World.h"
#include "Game/CoordSystems.h"
#include "Game/Terrain/TerrainData.h"

#include "Utils/Random.h"
#include "Utils/RectPoints.h"

namespace Expanse::Game::Terrain
{
	GenerateCells::GenerateCells(World& w, uint32_t seed)
		: ISystem(w)
		, types_seed(seed)
	{
		heights_seed = Xorshift32(seed);
	}

	void GenerateCells::Update()
	{
		if (!init)
		{
			init = true;

			static constexpr TerrainType TerrainTypes[] = { TerrainType::Dirt, TerrainType::Grass };

			for (const auto chunk_pos : utils::rect_points{ Rect{-2, -2, 4, 4} })
			{
				auto ent = world.entities.CreateEntity();
				auto* chunk = world.entities.AddComponent<TerrainChunk>(ent, chunk_pos);

				for (const auto local_pos : utils::rect_points{ chunk->cells.GetRect() })
				{
					const auto cell_pos = Coords::LocalToCell(local_pos, chunk_pos, TerrainChunk::Size);

					auto& cell = chunk->cells[local_pos];
					cell.type = TerrainTypes[NoiseInt(cell_pos, 0, 1, types_seed)];
					cell.height = NoiseInt(cell_pos, -4, 4, heights_seed);
				}
			}	
		}
	}
}