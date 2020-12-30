#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <util.h>

using IngredientSet = std::unordered_set<std::string>;
using AllergenMap = std::map<std::string, IngredientSet>;

struct Food {
	std::vector<std::string> ingredients;
	std::vector<std::string> allergens;

	Food(const std::string& line) {
		const size_t line_size = line.size();
		const size_t split_index = line.find('(');
		const std::string ingredients_str = line.substr(0, split_index - 1);
		util::Tokenize(ingredients_str, ' ',
			[this](const std::string& token, const int index) {
				ingredients.push_back(token);
			});

		const size_t allergen_index = split_index + 9;
		const std::string allergens_str = line.substr(
			allergen_index, line_size - allergen_index - 1);
		util::Tokenize(allergens_str, ',',
			[this](const std::string& token, const int index) {
				allergens.push_back(token.substr(1));
			});
	}
};

void TryPruneAllergenSets(AllergenMap& map, const std::string& allergen) {
	std::vector<std::string> prune_set = {allergen};

	while (!prune_set.empty()) {
		const std::string prune = std::move(prune_set.back());
		prune_set.pop_back();

		const IngredientSet& set = map.at(prune);
		if (set.size() != 1) {
			continue;
		}

		const std::string& ingredient = *(set.begin());
		for (auto& [key, value] : map) {
			if (value.size() == 1) {
				continue;
			}

			if (value.erase(ingredient) != 0) {
				prune_set.push_back(key);
			}
		}
	}
}

int main() {
	const std::vector<std::string> input = util::ReadInput();

	AllergenMap allergens;
	std::vector<Food> foods;

	for (const std::string& line : input) {
		Food food(line);

		for (const std::string& allergen : food.allergens) {
			if (allergens.contains(allergen)) {
				const IngredientSet& ingredient_set = allergens.at(allergen);
				IngredientSet new_ingredient_set;

				for (const std::string& ingredient : food.ingredients) {
					if (ingredient_set.contains(ingredient)) {
						new_ingredient_set.insert(ingredient);
					}
				}

				allergens.insert_or_assign(
					allergen, std::move(new_ingredient_set));
				TryPruneAllergenSets(allergens, allergen);
			} else {
				allergens.emplace(allergen, IngredientSet(
					food.ingredients.begin(), food.ingredients.end()));
				TryPruneAllergenSets(allergens, allergen);
			}
		}

		foods.push_back(std::move(food));
	}

	const std::string result = std::transform_reduce(
		allergens.begin(), allergens.end(), std::string(),
		[](const std::string& a, const std::string& b) -> std::string {
			return a.empty() ? b : a + "," + b;
		}, [](const std::pair<std::string, IngredientSet> a) {
			return *(a.second.begin());
		});
	std::cout << result << std::endl;
	return 0;
}
