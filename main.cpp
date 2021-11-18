#include <algorithm>
#include <array>
#include <iostream>
#include <iomanip>
#include <string>

using std::cout;
using std::setw;

using Card = int;
enum HandTypes {
    STRAIGHT_FLUSH, TRIPS, STRAIGHT, FLUSH, PAIR, HIGH_CARD, HANDTYPES_MAX
};

std::array<int, HANDTYPES_MAX> payouts = {100, 50, 9, 4, 1, 0};

const int NumberOfHandTypes = 6;
static_assert(NumberOfHandTypes == HANDTYPES_MAX);

std::array<std::string, NumberOfHandTypes> handRankNames = {"Straight flush", "Trips", "Straight", "Flush", "Pair",
                                                            "High card"};

int rank(Card c) { return c % 13; }

int suit(Card c) { return c / 13; }

bool isTrips(Card c1, Card c2, Card c3) { return rank(c1) == rank(c2) && rank(c2) == rank(c3); }

bool hasPair(Card c1, Card c2, Card c3) { return rank(c1) == rank(c2) || rank(c2) == rank(c3) || rank(c1) == rank(c3); }

bool isFlush(Card c1, Card c2, Card c3) { return suit(c1) == suit(c2) && suit(c2) == suit(c3); }

bool isStraight(Card c1, Card c2, Card c3) {
    std::array<int, 3> ranks = {rank(c1), rank(c2), rank(c3)};
    std::sort(begin(ranks), end(ranks));
    return (ranks[0] == 0 && ranks[1] == 11 && ranks[2] == 12) // AQK
           || (ranks[1] - 1 == ranks[0] && ranks[2] - 2 == ranks[0]); //other straights
}

HandTypes rankHand(Card c1, Card c2, Card c3) { //returns the highest rank that applies
    // Note that the order we check these hands is significant, for instance, trips are also a pair
    // and a straight flush is also a flush, so we check hand types in order from highest to lowest
    if (isFlush(c1, c2, c3) && isStraight(c1, c2, c3))
        return STRAIGHT_FLUSH;
    else if (isTrips(c1, c2, c3))
        return TRIPS;
    else if (isStraight(c1, c2, c3))
        return STRAIGHT;
    else if (isFlush(c1, c2, c3))
        return FLUSH;
    else if (hasPair(c1, c2, c3))
        return PAIR;
    else
        return HIGH_CARD;
}

int main() {
    std::array<int, NumberOfHandTypes> frequencies = {0};
    int handsConsidered = 0;
    for (int c1 = 0; c1 < 52; ++c1)
        for (int c2 = c1 + 1; c2 < 52; ++c2)
            for (int c3 = c2 + 1; c3 < 52; ++c3) {
                ++handsConsidered;
                ++frequencies[rankHand(c1, c2, c3)];
            }

    cout << std::fixed << std::setprecision(8);
    cout << "       Hand type   Frequency   Probability   Payout      Return\n";
    double totalReturn = 0;
    for (int handType = STRAIGHT_FLUSH; handType < HANDTYPES_MAX; ++handType) {
        double probability = double(frequencies[handType]) / handsConsidered;
        double returnEV = probability * payouts[handType];
        totalReturn += returnEV;
        cout << setw(16) << handRankNames[handType] << setw(12) << frequencies[handType]
             << setw(14) << probability << setw(9) << payouts[handType] << setw(12) << returnEV << "\n";
    }
    cout << "Total Return:" << setw(50) << totalReturn << "\n";
    return 0;
}
