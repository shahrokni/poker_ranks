#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

struct Card
{
    int rank;
    char suit;
};

int is_pattern_valid(const char *combination)
{
    /* A valid pattern contains 21 characters.
       Divided by 3, the rank and suit of each card should be determined.
       The 5 first groups represent the community cards, whereas the remaining represents the hole cards
       NOTE: 14 stands for aces
       EXAMPLE: 14H13H12H11H10H03D02S
    */
    int is_valid = TRUE;
    int combo_len = strlen(combination);

    if (combo_len != 21)
        is_valid = FALSE;

    if (is_valid)
    {
        for (int i = 0; i < 21; i += 3)
        {
            char num_one = combination[i];
            char num_two = combination[i + 1];
            char suite = combination[i + 2];

            char rank_number[3];
            rank_number[0] = num_one;
            rank_number[1] = num_two;
            rank_number[2] = '\0';

            int actual_rank = atoi(rank_number);

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
    int card_idx = 0;

    for (int i = 0; i < 21; i += 3)
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

int is_royal_flush(const struct Card *cards)
{
    /*
        Royal flush means 5 cards of the same suite from 10 to ace
        EXAMPLE: ACE, KING, QUEEN, JACK, and 10 of spades
        If the total_score is 10 + 11 + 12 + 13 + 14 = 60 and no iterated card is less than 10,
        we can assert royal flush exists
     */

    /* 0 > S 1 > H 2 > C 3 > D */
    int scores[4] = {0, 0, 0, 0};

    for (int j = 0; j < 7; j += 1)
    {
        struct Card card = cards[j];
        if (card.rank < 10)
            continue;

        int score_index = 0;

        if (card.suit == 'H')
            score_index = 1;
        else if (card.suit == 'C')
            score_index = 2;
        else if (card.suit == 'D')
            score_index = 3;

        scores[score_index] += card.rank;
    }

    for (int i = 0; i < 4; i += 1)
    {
        if (scores[i] == 60)
            return TRUE;
    }

    return FALSE;
}

int is_straight_flush(const struct Card *cards)
{
    int spades_bucket[13] = {0};
    int consecutive_spades_count = 0;

    int hearts_bucket[13] = {0};
    int consecutive_hearts_count = 0;

    int clubs_bucket[13] = {0};
    int consecutive_clubs_count = 0;

    int diamonds_bucket[13] = {0};
    int consecutive_diamonds_count = 0;

    for(int i=0;i<7;i+=1)
    {
        int rank = cards[i].rank;
        char suit = cards[i].suit;

        if(suit == 'S')
            spades_bucket[rank-2] = 1;
        else if(suit == 'H')
            hearts_bucket[rank-2] = 1;
        else if(suit == 'C')
            clubs_bucket[rank-2] = 1;
        else
            diamonds_bucket[rank-2] = 1;
    }

    for(int i=0;i<14;i+=1)
    {
        if(!spades_bucket[i])
            consecutive_spades_count = 0;
        else
            consecutive_spades_count +=1;
        if(!hearts_bucket[i])
            consecutive_hearts_count = 0;
        else
            consecutive_hearts_count+=1;
        if(!clubs_bucket[i])
            consecutive_clubs_count = 0;
        else
            consecutive_clubs_count+=1;
        if(!diamonds_bucket[i])
            consecutive_diamonds_count = 0;
        else
            consecutive_diamonds_count+=1;

        int any =
        consecutive_spades_count == 5 ||
        consecutive_hearts_count == 5 ||
        consecutive_clubs_count == 5 ||
        consecutive_diamonds_count == 5;

        if(any)
            return TRUE;
    }

    return FALSE;
}

int is_four_kind(const struct Card *cards)
{
   
}

void calculate_poker_rank(char *combination)
{
    if (!is_pattern_valid(combination))
    {
        printf("The pattern %s is not valid! \n", combination);
        return;
    }

    struct Card *cards = convert_combination_to_cards(combination);

    /* ROYAL FLUSH CHECK */
    if (is_royal_flush(cards))
    {
        printf("ROYAL FLUSH WAS FOUND: %s \n", combination);
        return;
    } else if(is_straight_flush(cards))
    {
        printf("STRAIGHT FLUSH WAS FOUND: %s \n",combination);
        return;
    }
}

int main()
{
    printf("POKER RANKING! \n");
    printf("-------------- \n");
    /* TEST ROYAL FLUSH */
    calculate_poker_rank("14H13H12H11H10H03D02S");
    calculate_poker_rank("14H12H13H10H11H03D02S");
    calculate_poker_rank("03D14H12H13H10H02S11H");

    /* TEST STRAIGHT FLUSH */
    calculate_poker_rank("08H07H06H05H04H03D02S");
    calculate_poker_rank("03D12S04D05D06D11C07D");
    calculate_poker_rank("04S02D02S03S14D06S05S");
    calculate_poker_rank("09S10S11S12S13S04D05D");
    calculate_poker_rank("08H07H06H05H04D03D02S"); // NOT!

    return 0;
}