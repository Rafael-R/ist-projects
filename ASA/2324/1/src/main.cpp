#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

class Piece
{
public:
    int width;
    int height;
    int price;
    Piece(int width, int height, int price)
    {
        this->width = width;
        this->height = height;
        this->price = price;
    }
    void print()    // debug
    {
        std::cout << this->width << " "
                  << this->height << " "
                  << this->price << std::endl;
    }
};

// Auxiliary function to sort pieces in descending order of price per square meter
bool compareByRatio(const Piece &a, const Piece &b)
{
    return a.price / (a.width * a.height) > b.price / (b.width * b.height);
}


int algorithm(std::pair<int, int> slab,
              std::vector<Piece> &pieces,
              std::map<std::pair<int, int>, int> &memo)
{
    // Swap the pair positions to avoid duplication in transposed slabs
    if (slab.first > slab.second) {
        std::swap(slab.first, slab.second);
    }

    if (memo.find(slab) != memo.end()) {
        return memo[slab];
    }

    int width = slab.first, height = slab.second, cut_width, cut_height, rotate;

    for (const auto &piece : pieces) {
        // When the pieces are square there is no need to rotate them
        rotate = (piece.width == piece.height) ? 0 : 1;

        // Try both orientations if not a square piece
        for (; rotate >= 0; rotate--) {
            if (rotate == 1) {
                cut_width = piece.width;
                cut_height = piece.height;
            } else {
                cut_width = piece.height;
                cut_height = piece.width;
            }

            if (cut_width <= width && cut_height <= height) {
                int remaining_width = width - cut_width;
                int remaining_height = height - cut_height;
                int profit;

                if (remaining_width == 0 && remaining_height == 0) {
                    profit = piece.price;
                } else if (remaining_width > 0 && remaining_height == 0) {
                    std::pair<int, int> sub_slab(remaining_width, height);
                    profit = piece.price + algorithm(sub_slab, pieces, memo);
                } else if (remaining_width == 0 && remaining_height > 0) {
                    std::pair<int, int> sub_slab(width, remaining_height);
                    profit = piece.price + algorithm(sub_slab, pieces, memo);
                } else {
                    // Vertical cut (vc)
                    std::pair<int, int> vc_sub_slab_1(remaining_width, height);
                    std::pair<int, int> vc_sub_slab_2(cut_width, remaining_height);
                    int vc_profit = algorithm(vc_sub_slab_1, pieces, memo) +
                                    algorithm(vc_sub_slab_2, pieces, memo);

                    // Horizontal cut (hc)
                    std::pair<int, int> hc_sub_slab_1(width, remaining_height);
                    std::pair<int, int> hc_sub_slab_2(remaining_width, cut_height);
                    int hc_profit = algorithm(hc_sub_slab_1, pieces, memo) +
                                    algorithm(hc_sub_slab_2, pieces, memo);

                    profit = piece.price + std::max(vc_profit, hc_profit);
                }
                memo[slab] = std::max(memo[slab], profit);
            }
        }
    }

    return memo[slab];
}

int algorithm_2(std::pair<int, int> slab, std::vector<Piece>& pieces) {
    std::map<std::pair<int, int>, int> memo;

    int cut_width, cut_height, rotate;

    if (slab.first < slab.second) {
        std::swap(slab.first, slab.second);
    }

    for (int height = 1; height <= slab.second; ++height) {
        for (int width = 1; width <= slab.first; ++width) {
            if (width >= height) {
                for (const auto& piece : pieces) {
                    // When the pieces are square there is no need to rotate them
                    rotate = (piece.width == piece.height) ? 0 : 1;

                    // Try both orientations if not a square piece
                    for (; rotate >= 0; rotate--) {
                        if (rotate == 1) {
                            cut_width = piece.width;
                            cut_height = piece.height;
                        } else {
                            cut_width = piece.height;
                            cut_height = piece.width;
                        }

                        if (cut_width <= width && cut_height <= height) {
                            int remaining_width = width - cut_width;
                            int remaining_height = height - cut_height;
                            int profit = piece.price 
                                       + std::max(memo[{remaining_width, height}] + 
                                                  memo[{cut_width, remaining_height}],
                                                  memo[{width, remaining_height}] + 
                                                  memo[{remaining_width, cut_height}]);
                            memo[{width, height}] = std::max(memo[{width, height}], profit);
                            memo[{height, width}] = std::max(memo[{height, width}], profit);
                        }
                    }
                }
            }
        }
    }

    return memo[slab];
}

int main()
{
    // Slab
    int width, height;
    std::cin >> width >> height;
    std::pair<int, int> slab(width, height);

    // Pieces
    int n, price;
    std::cin >> n;
    std::vector<Piece> pieces;
    for (int i = 0; i < n; i++) {
        std::cin >> width >> height >> price;
        pieces.push_back(Piece(width, height, price));
    }

    //std::map<std::pair<int, int>, int> memo;
    // int max_profit = algorithm(slab, pieces, memo);
    //std::cout << max_profit << std::endl;

    int max_profit_2 = algorithm_2(slab, pieces);
    std::cout << max_profit_2 << std::endl;

    return 0;
}
