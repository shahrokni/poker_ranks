#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define NOT_DETECTED "NOT DETECTED"

struct Card
{
    int rank;
    char suite;
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

// struct Card* convert_combo_to_cards(const char *combination)
// {
// }

void calculate_poker_rank(char *combination)
{
    if (!is_pattern_valid(combination))
    {
        printf("The pattern %s is not valid!", combination);
        return;
    }
    
    /* the rest goes here */
}

int main()
{
    printf("POKER RANKING! \n");
    return 0;
}