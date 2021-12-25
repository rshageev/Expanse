#include "ProceduralTerrain.h"

#include "Game/CoordSystems.h"

#include "Utils/Random.h"
#include "Utils/RectPoints.h"
#include "Utils/Utils.h"

namespace Expanse::Game::Terrain
{
	struct NoiseParam
	{
		float freq;
		float min;
		float max;
	};

	TerrainLoader_Procedural::TerrainLoader_Procedural(uint32_t seed)
	{
		types_seed = Squirrel3(0, seed);
		heights_seed[0] = Squirrel3(1, seed);
		heights_seed[1] = Squirrel3(2, seed);
	}

	bool TerrainLoader_Procedural::LoadChunk(TerrainChunk& chunk)
	{
		for (const auto local_pos : utils::rect_points{ chunk.cells.GetRect() })
		{
			const auto cell_pos = Coords::LocalToCell(local_pos, chunk.position, TerrainChunk::Size);

			auto& cell = chunk.cells[local_pos];

			cell.type = GetTerrainAt(cell_pos);
			cell.height = static_cast<int>(GetHeightAt(cell_pos));
		}

		return true;
	}

	float TerrainLoader_Procedural::GetHeightAt(Point cell_pos) const
	{
		static constexpr NoiseParam heights_noise_params[] = {
			{ 0.05f, -20.0f, 20.0f },
			{ 0.27f, -5.0f, 5.0f },
		};

		float height = 0.0f;
		utils::for_each_zipped(heights_seed, heights_noise_params, [&height, cell_pos](uint32_t seed, const auto& params)
		{
			const auto pt = FPoint{ cell_pos } *params.freq;
			height += Lerp(params.min, params.max, PerlinNoise(pt, seed));
		});

		return height;
	}

	TerrainType TerrainLoader_Procedural::GetTerrainAt(Point cell_pos) const
	{
		const auto pt = FPoint{ cell_pos } *0.07f;
		const float n = PerlinNoise(pt, types_seed);

		return (n > 0.0f) ? TerrainType::Grass : TerrainType::Dirt;
	}
}