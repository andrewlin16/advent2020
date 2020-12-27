#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <util.h>

using EdgeData = std::bitset<10>;

enum Edge {
	kNorthEdge = 0,
	kEastEdge = 1,
	kSouthEdge = 2,
	kWestEdge = 3,
};

constexpr Edge GetOppositeEdge(const Edge edge) {
	return static_cast<Edge>(static_cast<int>(edge) ^ 2);
}

template<typename T>
constexpr T GetRoot(T n) {
	return static_cast<T>(std::sqrt(n));
}

// These aren't used by their specific named values, but might as well define
// the 8 orientations as an enum anyway.
enum Orientation {
	kCwTL = 0,
	kCwTR = 1,
	kCwBL = 2,
	kCwBR = 3,
	kCcwTL = 4,
	kCcwTR = 5,
	kCcwBL = 6,
	kCcwBR = 7,
};

struct EdgeOrientation {
	Edge edge;
	bool flip;
};

// Type which defines which edge maps to which edge for a particular
// orientation and whether the edge needs flipping.
using OrientationMap = std::array<EdgeOrientation, 4>;

const std::array<OrientationMap, 8> kOrientations = {{
	{{	// CW top-left
		{kNorthEdge, false},
		{kEastEdge, false},
		{kSouthEdge, false},
		{kWestEdge, false},
	}},
	{{	// CW top-right
		{kWestEdge, true},
		{kNorthEdge, false},
		{kEastEdge, true},
		{kSouthEdge, false},
	}},
	{{	// CW bottom-right
		{kSouthEdge, true},
		{kWestEdge, true},
		{kNorthEdge, true},
		{kEastEdge, true},
	}},
	{{	// CW bottom-left
		{kEastEdge, false},
		{kSouthEdge, true},
		{kWestEdge, false},
		{kNorthEdge, true},
	}},
	{{	// CCW top-left
		{kWestEdge, false},
		{kSouthEdge, false},
		{kEastEdge, false},
		{kNorthEdge, false},
	}},
	{{	// CCW top-right
		{kNorthEdge, true},
		{kWestEdge, false},
		{kSouthEdge, true},
		{kEastEdge, false},
	}},
	{{	// CCW bottom-right
		{kEastEdge, true},
		{kNorthEdge, true},
		{kWestEdge, true},
		{kSouthEdge, true},
	}},
	{{	// CCW bottom-left
		{kSouthEdge, false},
		{kEastEdge, true},
		{kNorthEdge, false},
		{kWestEdge, true},
	}},
}};

class Tile {
public:
	Tile(const std::vector<std::string>& input) {
		const std::string& tile_id_line = input[0];
		const size_t start_index = tile_id_line.find(' ') + 1;
		const size_t end_index = tile_id_line.find(':', start_index);
		id = std::stoi(tile_id_line.substr(
			start_index, end_index - start_index));

		edges[kNorthEdge] = EdgeData(input[1], 0, 10, '.', '#');
		edges[kSouthEdge] = EdgeData(input[10], 0, 10, '.', '#');

		for (int i = 0; i < 10; ++i) {
			const std::string& tile_line = input[i + 1];
			edges[kWestEdge][9 - i] = (tile_line[0] == '#');
			edges[kEastEdge][9 - i] = (tile_line[9] == '#');
		}

		for (int k = 0; k < 4; ++k) {
			const EdgeData& edge = edges[k];
			EdgeData& flipped_edge = flipped_edges[k];

			for (int i = 0; i < 10; ++i) {
				flipped_edge[i] = edge[9 - i];
			}
		}
	}

	const EdgeData& GetEdge(
			const Edge edge, const Orientation orientation) const {
		const OrientationMap& orient_map = kOrientations[orientation];
		const EdgeOrientation& edge_orient = orient_map[edge];

		if (edge_orient.flip) {
			return flipped_edges[edge_orient.edge];
		} else {
			return edges[edge_orient.edge];
		}
	}

	const int GetId() const {
		return id;
	}

private:
	int id;
	std::array<EdgeData, 4> edges;
	std::array<EdgeData, 4> flipped_edges;
};

struct OrientedTile {
	size_t tile;
	Orientation orientation;
};

bool HasTile(const std::vector<OrientedTile>& tiles, const size_t tile) {
	return std::any_of(tiles.begin(), tiles.end(),
		[&tile](const OrientedTile t) {
			return t.tile == tile;
		});
}

struct Constraint {
	int index;
	Edge edge;
};

// Solving occurs in this order for example (from 0 to F for a 4x4):
//
// 0149
// 325A
// 876B
// FEDC
//
// This has a balance of not requiring too complicated math to determine what
// has to match with what while trying to get as many constraints as possible
// early for pruning the search.
std::vector<OrientedTile> FindValidArrangement(const std::vector<Tile>& tiles) {
	const size_t num_tiles = tiles.size();
	std::vector<std::vector<OrientedTile>> search;

	// Start with each tile at top-left corner with each possible orientation.
	for (size_t t = 0; t < num_tiles; ++t) {
		for (int o = 0; o < 8; ++o) {
			search.emplace_back(
				1, OrientedTile(t, static_cast<Orientation>(o)));
		}
	}

	// DFS for matching arrangements.
	while (!search.empty()) {
		std::vector<OrientedTile> candidate = std::move(search.back());
		search.pop_back();

		// See which step this candidate is on.
		const size_t step = candidate.size();
		const size_t layer = GetRoot(step);

		if (step == tiles.size()) {
			// Once all tiles have been arranged in a valid manner, return the
			// arrangement.
			return candidate;
		}

		// There are 4 possible areas of the layer that this step could be
		// in, each with different constraints.
		std::vector<Constraint> constraints;
		if (step == layer * layer) {
			// 1. Top-right corner: match tile to the left.
			const size_t left_index = step - (2 * layer - 1);
			constraints = {
				Constraint(left_index, kEastEdge),
			};
		} else if (step < layer * (layer + 1)) {
			// 2. Right side: match tile above and to the left.
			const size_t top_index = step - 1;
			const size_t left_index = step - (2 * layer - 1);
			constraints = {
				Constraint(top_index, kSouthEdge),
				Constraint(left_index, kEastEdge),
			};
		} else if (step == layer * (layer + 1)) {
			// 3. Bottom-right corner: match tile above
			const size_t top_index = step - 1;
			constraints = {
				Constraint(top_index, kSouthEdge),
			};
		} else {
			// 4. Bottom side: match tile above and to the right.
			const size_t top_index = step - (2 * layer + 1);
			const size_t right_index = step - 1;
			constraints = {
				Constraint(top_index, kSouthEdge),
				Constraint(right_index, kWestEdge),
			};
		}

		for (size_t t = 0; t < num_tiles; ++t) {
			// Don't try tiles that have already been used in the candidate.
			if (HasTile(candidate, t)) {
				continue;
			}

			const Tile& new_tile = tiles[t];
			for (int k = 0; k < 8; ++k) {
				const Orientation o = static_cast<Orientation>(k);

				// This tile fits with orientation O if all edge constraints
				// are met.
				const bool matches = std::all_of(
					constraints.begin(), constraints.end(),
					[&candidate, &tiles, &new_tile, o](const Constraint c) {
						const int index = c.index;
						const OrientedTile tile = candidate[index];
						const Tile& target_tile = tiles[tile.tile];
						const Edge match_edge = c.edge;

						const EdgeData& target_edge = target_tile.GetEdge(
							match_edge, tile.orientation);
						const EdgeData& new_edge = new_tile.GetEdge(
							GetOppositeEdge(match_edge), o);
						return target_edge == new_edge;
					});

				if (matches) {
					// Tile fits, add this tile arrangement to search stack.
					std::vector<OrientedTile> new_candidate = candidate;
					new_candidate.emplace_back(t, o);
					search.push_back(std::move(new_candidate));
				}
			}
		}
	}

	throw std::runtime_error("exhausted search without finding solution");
}

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();
	std::vector<Tile> tiles;

	std::transform(input.begin(), input.end(), std::back_inserter(tiles),
		[](const std::vector<std::string>& lines) -> Tile {
			return Tile(lines);
		});
	const std::vector<OrientedTile> solution = FindValidArrangement(tiles);

	const size_t num_tiles = tiles.size();
	const size_t root = GetRoot(num_tiles);
	const std::array<size_t, 4> corner_indexes = {
		0,	// top-left
		(root - 1) * (root - 1),	// top-right
		(root - 1) * root,	// bottom-right
		num_tiles - 1,	// bottom-left
	};
	const unsigned long result = std::transform_reduce(
		corner_indexes.begin(), corner_indexes.end(), 1UL,
		std::multiplies<unsigned long>(),
		[&tiles, &solution](const int i) -> unsigned long {
			const OrientedTile ot = solution[i];
			const Tile& tile = tiles[ot.tile];
			return static_cast<unsigned long>(tile.GetId());
		});

	std::cout << result << std::endl;
	return 0;
}
