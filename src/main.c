#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* APPLICATION */
#define TRUE 1
#define FALSE 0
#define MALLOC_FAILURE_EXCEPTION_MESSAGE "MALLOC_FAILURE_EXCEPTION"

/* POKER */
#define NVP "NOT A VALID PATTERN"
#define RF "ROYAL FLUSH"
#define SF "STRAIGHT FLUSH"
#define FOK "FOUR OF A KIND"
#define FH "FULL HOUSE"
#define F "FLUSH"
#define S "STRAIGHT"
#define TOK "THREE OF A KIND"
#define TP "TWO PARIS"
#define P "PAIR"
#define HG "HIGH CARD"
#define SPADES_SYMBOL "♠"
#define HEARTS_SYMBOL "♥"
#define CLUBS_SYMBOL "♣"
#define DIAMONDS_SYMBOL "♦"

struct Card
{
    short rank;
    char suit;
};

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

    /* 0=>S 1=>H 2=>C 3=>D */
    short scores[4] = {0};

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

        if (
            consecutive_spades_count == 5 ||
            consecutive_hearts_count == 5 ||
            consecutive_clubs_count == 5 ||
            consecutive_diamonds_count == 5)
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
        We use a bucket of the ranks to count the cards per rank
        The rule is that four of a kind can't be split into smaller groups.
        Thus, if you have four cards of the same rank,
        you can't consider part of it as a three of a kind and another part as a pair.
     */
    short ranks[13] = {0};

    for (short i = 0; i < 7; i += 1)
    {
        short rank_index = cards[i].rank - 2;
        ranks[rank_index] += 1;
    }

    short has_3_kind = FALSE;
    short has_pair = FALSE;

    for (short i = 0; i < 13; i += 1)
    {
        if (!has_3_kind)
            has_3_kind = ranks[i] == 3;
        if (!has_pair)
            has_pair = ranks[i] == 2;

        if (has_3_kind && has_pair)
            return TRUE;
    }

    return FALSE;
}

short is_flush(const struct Card *cards)
{
    /* 0=>S 1=>H 2=>C 3=>D */
    short suits_count[4] = {0};

    for (short i = 0; i < 7; i += 1)
    {
        char suit = cards[i].suit;

        if (suit == 'S')
            suits_count[0] += 1;
        else if (suit == 'H')
            suits_count[1] += 1;
        else if (suit == 'C')
            suits_count[2] += 1;
        else
            suits_count[3] += 1;

        if (suits_count[0] == 5 ||
            suits_count[1] == 5 ||
            suits_count[2] == 5 ||
            suits_count[3] == 5)
            return TRUE;
    }

    return FALSE;
}

short is_straight(const struct Card *cards)
{
    short ranks[13] = {0};
    short count_consecutive_ranks = 0;

    for (short i = 0; i < 7; i += 1)
        ranks[cards[i].rank - 2] = 1;

    for (short i = 0; i < 13; i += 1)
    {
        if (ranks[i])
            count_consecutive_ranks += 1;
        else
            count_consecutive_ranks = 0;

        if (count_consecutive_ranks == 5)
            return TRUE;
    }

    return FALSE;
}

short is_three_kind(const struct Card *cards)
{
    short ranks[13] = {0};

    for (short i = 0; i < 7; i += 1)
        ranks[cards[i].rank - 2] += 1;

    for (short i = 0; i < 13; i += 1)
        if (ranks[i] == 3)
            return TRUE;

    return FALSE;
}

short is_x_pairs(const struct Card *cards, const short expected_pair)
{
    short ranks[13] = {0};

    for (short i = 0; i < 7; i += 1)
        ranks[cards[i].rank - 2] += 1;

    short count_pair = 0;
    for (short i = 0; i < 13; i += 1)
    {
        if (ranks[i] == 2)
            count_pair += 1;

        if (count_pair == expected_pair)
            return TRUE;
    }

    return FALSE;
}

char *calculate_poker_rank(char *id, char *combination)
{
    if (!is_pattern_valid(combination))
    {
        printf("The pattern %s is not valid! \n", combination);
        return NVP;
    }

    struct Card *cards = convert_combination_to_cards(combination);

    /* ROYAL FLUSH CHECK */
    if (is_royal_flush(cards))
    {
        return RF;
    }
    else if (is_straight_flush(cards))
    {
        return SF;
    }
    else if (is_four_kind(cards))
    {
        return FOK;
    }
    else if (is_full_house(cards))
    {
        return FH;
    }
    else if (is_flush(cards))
    {
        return F;
    }
    else if (is_straight(cards))
    {
        return S;
    }
    else if (is_three_kind(cards))
    {
        return TOK;
    }
    else if (is_x_pairs(cards, 2))
    {
        return TP;
    }
    else if (is_x_pairs(cards, 1))
    {
        return P;
    }

    return HG;
}

void set_card_str(char **combinations, short combo_index, short current_pattern_len, short rank, short suit)
{
    const char suits[4] = {'S', 'H', 'C', 'D'};
    char str_rank[3];
    if (rank < 10)
        sprintf(str_rank, "%02d", rank);
    else
        sprintf(str_rank, "%d", rank);

    combinations[combo_index][current_pattern_len] = str_rank[0];
    combinations[combo_index][current_pattern_len + 1] = str_rank[1];
    combinations[combo_index][current_pattern_len + 2] = suits[suit];
}

char **generate_random_combinations(short count_combinations)
{
    /*
        Range of the rank is from 2 to 14 (ACE)
        To calculate the equal index of the array, the rank is subtracted from 2
        To  calculate the rank based on the index, the index is increased by 2
    */

    char **combinations = malloc(count_combinations * sizeof(char *));
    if (combinations == NULL)
    {
        printf(MALLOC_FAILURE_EXCEPTION_MESSAGE);
        exit(1);
    }

    for (short i = 0; i < count_combinations; i += 1)
    {
        combinations[i] = malloc(22 * sizeof(char));
        if (combinations[i] == NULL)
        {
            printf(MALLOC_FAILURE_EXCEPTION_MESSAGE);
            exit(1);
        }
    }

    short count_generated_patterns = 0;
    short current_pattern_length = 0;
    short generated_spades_rank[13] = {0};
    short generated_hearts_rank[13] = {0};
    short generated_clubs_rank[13] = {0};
    short generated_diamonds_rank[13] = {0};

    short *all_generated_ranks[] = {generated_spades_rank,
                                    generated_hearts_rank,
                                    generated_clubs_rank,
                                    generated_diamonds_rank};

    srand(time(0));

    while (count_generated_patterns < count_combinations)
    {
        while (current_pattern_length != 21)
        {

            short random_suit = rand() % 4;
            short random_rank = (rand() % 13) + 2;

            if (!all_generated_ranks[random_suit][random_rank - 2])
            {
                all_generated_ranks[random_suit][random_rank - 2] = 1;
                set_card_str(combinations,
                             count_generated_patterns,
                             current_pattern_length,
                             random_rank,
                             random_suit);
                current_pattern_length += 3;
            }
            else
                continue;
        }
        combinations[count_generated_patterns][21] = '\0';
        current_pattern_length = 0;
        count_generated_patterns += 1;
        for (short i = 0; i < 4; i += 1)
            for (short j = 0; j < 13; j += 1)
                all_generated_ranks[i][j] = 0;
    }

    return combinations;
}

char* get_suit_symbol(char suit)
{
    if(suit == 'S')
        return SPADES_SYMBOL;
    else if(suit == 'H')
        return HEARTS_SYMBOL;
    else if(suit == 'C')
        return CLUBS_SYMBOL;
    else
        return DIAMONDS_SYMBOL;
}

void beautify_combination(const char *combination)
{       
    for (short i = 0; i < 21; i += 3)
    {
        char rank_char_arr[3] = {combination[i], combination[i + 1], '\0'};        
        short rank = atoi(rank_char_arr);

        if(rank<10)
        {
            char rank_str[2];
            sprintf(rank_str,"%d",rank);
            printf(" %s%s ",rank_str,get_suit_symbol(combination[i + 2]));
        }
        else if(rank == 10)
        {
            printf(" 10%s ",get_suit_symbol(combination[i + 2]));
        }
        else
        {
            char symbol;
            switch (rank)
            {
            case 11:
                symbol = 'J';
                break;
            case 12:
                symbol = 'Q';
                break;
            case 13:
                symbol = 'K';
                break;
            case 14:
                symbol = 'A';
                break;            
            default:
                break;
            }
            printf(" %c%s ",symbol,get_suit_symbol(combination[i + 2]));
        }
    }
    printf("\n");
}

int main()
{
    char **combos = generate_random_combinations(20);

    for (short i = 0; i < 20; i += 1)
    {        
        char id[20];       
        printf(" %s \n\n", calculate_poker_rank(id, combos[i]));
        beautify_combination(combos[i]);
        printf("....................\n\n"); 
    }

    return 0;
}