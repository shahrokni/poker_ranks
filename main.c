#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

struct Card
{
    short rank;
    char suit;
};
void drw_line()
{
    printf("............................... \n");
}

short is_pattern_valid(const char *combination)
{
    /* A valid pattern contains 21 characters.
       Divided by 3, the rank and suit of each card should be determined.
       The 5 first groups represent the community cards, whereas the remaining represents the hole cards
       NOTE: 14 stands for aces
       EXAMPLE: 14H13H12H11H10H03D02S
    */
    short is_valid = TRUE;
    short combo_len = strlen(combination);

    if (combo_len != 21)
        is_valid = FALSE;

    if (is_valid)
    {
        for (short i = 0; i < 21; i += 3)
        {
            char num_one = combination[i];
            char num_two = combination[i + 1];
            char suite = combination[i + 2];

            char rank_number[3];
            rank_number[0] = num_one;
            rank_number[1] = num_two;
            rank_number[2] = '\0';

            short actual_rank = atoi(rank_number);

            if (actual_rank == 0 || actual_rank < 2 || actual_rank > 14)
            {
                is_valid = FALSE;
                break;
            }

            if (suite != 'H' && suite != 'S' && suite != 'D' && suite != 'C')
            {
                is_valid = FALSE;
                break;
            }
        }
    }

    return is_valid;
}

struct Card *convert_combination_to_cards(const char *combination)
{
    struct Card *cards = malloc(7 * sizeof(struct Card));
    short card_idx = 0;

    for (short i = 0; i < 21; i += 3)
    {
        char rank_number[3];
        rank_number[0] = combination[i];
        rank_number[1] = combination[i + 1];
        rank_number[2] = '\0';

        char suite = combination[i + 2];

        cards[card_idx].rank = atoi(rank_number);
        cards[card_idx].suit = suite;

        card_idx += 1;
    }

    return cards;
}

short is_royal_flush(const struct Card *cards)
{
    /*
        Royal flush means 5 cards of the same suite from 10 to ace
        EXAMPLE: ACE, KING, QUEEN, JACK, and 10 of spades
        If the total_score is 10 + 11 + 12 + 13 + 14 = 60 and no iterated card is less than 10,
        we can assert royal flush exists
     */

    /* 0 > S 1 > H 2 > C 3 > D */
    short scores[4] = {0, 0, 0, 0};

    for (short j = 0; j < 7; j += 1)
    {
        struct Card card = cards[j];
        if (card.rank < 10)
            continue;

        short score_index = 0;

        if (card.suit == 'H')
            score_index = 1;
        else if (card.suit == 'C')
            score_index = 2;
        else if (card.suit == 'D')
            score_index = 3;

        scores[score_index] += card.rank;
    }

    for (short i = 0; i < 4; i += 1)
    {
        if (scores[i] == 60)
            return TRUE;
    }

    return FALSE;
}

short is_straight_flush(const struct Card *cards)
{
    short spades_bucket[13] = {0};
    short consecutive_spades_count = 0;

    short hearts_bucket[13] = {0};
    short consecutive_hearts_count = 0;

    short clubs_bucket[13] = {0};
    short consecutive_clubs_count = 0;

    short diamonds_bucket[13] = {0};
    short consecutive_diamonds_count = 0;

    for (short i = 0; i < 7; i += 1)
    {
        short rank = cards[i].rank;
        char suit = cards[i].suit;

        if (suit == 'S')
            spades_bucket[rank - 2] = 1;
        else if (suit == 'H')
            hearts_bucket[rank - 2] = 1;
        else if (suit == 'C')
            clubs_bucket[rank - 2] = 1;
        else
            diamonds_bucket[rank - 2] = 1;
    }

    for (short i = 0; i < 14; i += 1)
    {
        if (!spades_bucket[i])
            consecutive_spades_count = 0;
        else
            consecutive_spades_count += 1;
        if (!hearts_bucket[i])
            consecutive_hearts_count = 0;
        else
            consecutive_hearts_count += 1;
        if (!clubs_bucket[i])
            consecutive_clubs_count = 0;
        else
            consecutive_clubs_count += 1;
        if (!diamonds_bucket[i])
            consecutive_diamonds_count = 0;
        else
            consecutive_diamonds_count += 1;

        short any =
            consecutive_spades_count == 5 ||
            consecutive_hearts_count == 5 ||
            consecutive_clubs_count == 5 ||
            consecutive_diamonds_count == 5;

        if (any)
            return TRUE;
    }

    return FALSE;
}

short is_four_kind(const struct Card *cards)
{
    /*
        In the aggregate we have 7 cars including 5 community and 2 hole cards respectively
        If for each card we can find 3 other cards with the same rank, we can say the 4 of a kind exists
        We only need to check the 4 first cards to determine if 4 of a kind exists or not
        Beyond that index we can simply say the pattern does not exist because of the lack of enough cards
     */
    short count_same_ranks = 0;
    for (short i = 0; i < 4; i += 1)
    {
        for (short j = i + 1; j < 7; j += 1)
        {
            if ((7 - j) + count_same_ranks < 3)
                break;
            if (cards[i].rank == cards[j].rank)
                count_same_ranks += 1;
        }
        if (count_same_ranks == 3)
            return TRUE;
        else
            count_same_ranks = 0;
    }

    return FALSE;
}

short is_full_house(const struct Card *cards)
{
    /*
        Here we need to find 3 of a kind and a pair
        We use a bucket to count the cards per rank
        The rule is that four of a kind can't be split into smaller groups.
        Thus, if you have four cards of the same rank,
        you can't consider part of it as a three of a kind and another part as a pair.      
     */
    short bucket[13] = {0};

    for (short i = 0; i < 7; i += 1)
    {
        short bucket_index = cards[i].rank - 2;
        bucket[bucket_index] += 1;
    }

    short has_3_kind = FALSE;
    short has_pair = FALSE;

    for (short i = 0; i < 13; i += 1)
    {
        if(!has_3_kind)
            has_3_kind = bucket[i] == 3;
        if(!has_pair)
            has_pair = bucket[i] == 2;

        if (has_3_kind && has_pair)
            return TRUE;
    }  

    return FALSE;
}

void calculate_poker_rank(char *id, char *combination)
{
    if (!is_pattern_valid(combination))
    {
        printf("The pattern %s is not valid! \n", combination);
        return;
    }

    struct Card *cards = convert_combination_to_cards(combination);

    drw_line();
    printf("%s is being checked ... \n", combination);

    /* ROYAL FLUSH CHECK */
    if (is_royal_flush(cards))
    {
        printf("%s > ROYAL FLUSH WAS FOUND: %s \n", id, combination);
        drw_line();

        return;
    }
    else if (is_straight_flush(cards))
    {
        printf("%s > STRAIGHT FLUSH WAS FOUND: %s \n", id, combination);
        drw_line();

        return;
    }
    else if (is_four_kind(cards))
    {
        printf("%s > 4 OF A KIND WAS FOUND: %s \n", id, combination);
        drw_line();

        return;
    }
    else if (is_full_house(cards))
    {
        printf("%s > FULL HOUSE WAS FOUND: %s \n", id, combination);
        drw_line();

        return;
    }

    printf("%s > No pattern was found \n", id);
    drw_line();
}

int main()
{
    printf("POKER RANKING! \n");
    printf("-------------- \n");
    /* TEST ROYAL FLUSH */
    calculate_poker_rank("1", "14H13H12H11H10H03D02S");
    calculate_poker_rank("2", "14H12H13H10H11H03D02S");
    calculate_poker_rank("3", "03D14H12H13H10H02S11H");

    /* TEST STRAIGHT FLUSH */
    calculate_poker_rank("4", "08H07H06H05H04H03D02S");
    calculate_poker_rank("5", "03D12S04D05D06D11C07D");
    calculate_poker_rank("6", "04S02D02S03S14D06S05S");
    calculate_poker_rank("7", "09S10S11S12S13S04D05D");
    calculate_poker_rank("8", "08H07H06H05H04D03D02S");

    /* TEST 4 OF A KIND*/
    calculate_poker_rank("9", "05S05D05C05H14D12S12D");
    calculate_poker_rank("10", "12S05D12H03D12C02D12D");
    calculate_poker_rank("11", "02H03H04H13S13H13C13D");
    calculate_poker_rank("12", "02S02H10D11D12D02C02D");

    /* TEST FULL HOUSE */
    calculate_poker_rank("13", "13H13D13S05H05C02D02C");
    calculate_poker_rank("13", "02S02D02H03S03D03H12D");

    return 0;
}