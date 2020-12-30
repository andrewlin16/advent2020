#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <util.h>

using EdgeData = std::bitset<10>;
using TileBitmap = std::array<std::bitset<8>, 8>;
using Bitmap = std::vector<std::vector<bool>>;

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

enum Orientation {
	kCwTL = 0,
	kCwTR = 1,
	kCwBR = 2,
	kCwBL = 3,
	kCcwTL = 4,
	kCcwTR = 5,
	kCcwBR = 6,
	kCcwBL = 7,
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

// 0123456789abcdefghij
// ..................#.
// #....##....##....###
// .#..#..#..#..#..#...
const std::vector<std::vector<int>> kMonsterImage = {
	{18},
	{0, 5, 6, 11, 12, 17, 18, 19},
	{1, 4, 7, 10, 13, 16},
};

const size_t kMonsterImageWidth = 20;
const size_t kMonsterImageCount = 15;

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

		for (int i = 0; i < 8; ++i) {
			data[i] = std::bitset<8>(input[i + 2], 1, 8, '.', '#');
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

	bool GetBit(const int r, const int c, const Orientation o) const {
		switch (o) {
			case kCwTL:
				return data[r][7 - c];
				break;
			case kCwTR:
				return data[7 - c][7 - r];
				break;
			case kCwBR:
				return data[7 - r][c];
				break;
			case kCwBL:
				return data[c][r];
				break;
			case kCcwTL:
				return data[c][7 - r];
				break;
			case kCcwTR:
				return data[r][c];
				break;
			case kCcwBR:
				return data[7 - c][r];
				break;
			case kCcwBL:
				return data[7 - r][7 - c];
				break;
			default:
				throw std::runtime_error("invalid orientation");
		}
	}

private:
	int id;
	std::array<EdgeData, 4> edges;
	std::array<EdgeData, 4> flipped_edges;
	TileBitmap data;
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

std::vector<std::vector<OrientedTile>> ToGrid(
		const std::vector<OrientedTile>& v) {
	const size_t size = v.size();
	const size_t root = GetRoot(size);

	std::vector<std::vector<OrientedTile>> result(
		root, std::vector<OrientedTile>(root));

	for (size_t i = 0; i < root; ++i) {
		for (size_t k = 0; k < i; ++k) {
			result[k][i] = v[i * i + k];
		}
		for (size_t k = 0; k <= i; ++k) {
			result[i][i - k] = v[i * (i + 1) + k];
		}
	}

	return result;
}

Bitmap ArrangeBitmap(const std::vector<Tile>& tiles,
		const std::vector<OrientedTile>& v) {
	const std::vector<std::vector<OrientedTile>> tile_grid = ToGrid(v);
	const size_t root = tile_grid.size();
	const size_t size = root * 8;

	Bitmap bitmap(size, std::vector<bool>(size));

	for (size_t r = 0; r < root; ++r) {
		const std::vector<OrientedTile>& tile_grid_row = tile_grid[r];

		for (int i = 0; i < 8; ++i) {
			std::vector<bool>& row = bitmap[r * 8 + i];

			for (size_t c = 0; c < root; ++c) {
				const OrientedTile& ot = tile_grid_row[c];
				const Tile& tile = tiles[ot.tile];
				const Orientation orientation = ot.orientation;

				for (int k = 0; k < 8; ++k) {
					row[c * 8 + k] = tile.GetBit(i, k, orientation);
				}
			}
		}
	}


	return bitmap;
}

bool RowMatches(const std::vector<bool>& row, const std::vector<int> image,
		int offset, bool hflip) {
	const std::function<bool(int)> match_fun = (
		hflip ?
			std::function<bool(int)>([&row, offset](const int index) {
				return row[offset + kMonsterImageWidth - index];
			}) :
			std::function<bool(int)>([&row, offset](const int index) {
				return row[offset + index];
			}));

	return std::all_of(image.begin(), image.end(), match_fun);
}

int CountMonsters(const Bitmap& bitmap) {
	const size_t size = bitmap.size();

	int count = 0;
	std::optional<std::pair<bool, bool>> flip;

	for (size_t r = 1; r + 1 < size; ++r) {
		const std::vector<bool>& row = bitmap[r];

		for (size_t c = 0; c + kMonsterImageWidth - 1 < size; ++c) {
			// Check center row match first.
			bool hflip;

			if (flip.has_value()) {
				hflip = flip->first;
				if (!RowMatches(row, kMonsterImage[1], c, hflip)) {
					continue;
				}
			} else {
				if (RowMatches(row, kMonsterImage[1], c, false)) {
					hflip = false;
				} else if (RowMatches(row, kMonsterImage[1], c, true)) {
					hflip = true;
				} else {
					continue;
				}
			}

			// Now that center row matches, check for top/bottom row match.
			const std::vector<bool>& top_row = bitmap[r - 1];
			const std::vector<bool>& bot_row = bitmap[r + 1];
			bool vflip;

			if (flip.has_value()) {
				vflip = flip->second;
				if (!RowMatches(
						top_row, kMonsterImage[vflip ? 2 : 0], c, hflip) ||
					!RowMatches(
						bot_row, kMonsterImage[vflip ? 0 : 2], c, hflip)) {
					continue;
				}
			} else {
				if (RowMatches(top_row, kMonsterImage[0], c, hflip) &&
						RowMatches(bot_row, kMonsterImage[2], c, hflip)) {
					vflip = false;
				} else if (RowMatches(top_row, kMonsterImage[2], c, hflip) &&
						RowMatches(bot_row, kMonsterImage[0], c, hflip)) {
					vflip = true;
				} else {
					continue;
				}
			}

			// All 3 rows match, so increment count and set flip status.
			++count;
			if (!flip.has_value()) {
				flip = {hflip, vflip};
			}
		}
	}

	return count;
}

void FlipBitmap(Bitmap& bitmap) {
	const size_t size = bitmap.size();
	for (size_t i = 0; i < size; ++i) {
		for (size_t k = i + 1; k < size; ++k) {
			std::swap(bitmap[i][k], bitmap[k][i]);
		}
	}
}

int CountBits(const Bitmap& bitmap) {
	return std::transform_reduce(
		bitmap.begin(), bitmap.end(), 0, std::plus<int>(),
		[](const std::vector<bool> row) {
			return std::count(row.begin(), row.end(), true);
		});
}

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();
	std::vector<Tile> tiles;

	std::transform(input.begin(), input.end(), std::back_inserter(tiles),
		[](const std::vector<std::string>& lines) -> Tile {
			return Tile(lines);
		});
	const std::vector<OrientedTile> solution = FindValidArrangement(tiles);

	Bitmap bitmap = ArrangeBitmap(tiles, solution);
	int count = CountMonsters(bitmap);
	if (count == 0) {
		FlipBitmap(bitmap);
		count = CountMonsters(bitmap);
	}

	const int result = CountBits(bitmap) - kMonsterImageCount * count;
	std::cout << result << std::endl;
	return 0;
}
