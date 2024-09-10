# Poker ranks

**Poker Rank** is a simple C application for detecting poker hand rankings.
The application was primarily developed for educational purposes, providing the author with an opportunity to refresh and improve their C programming skills after a long hiatus. Avid poker players and developers interested in creating their own games are encouraged to take advantage of this repository.

![DEMO](https://github.com/shahrokni/poker_ranks/blob/master/demo/demo-1.png)

# What are Poker Hand Rankings?

In a standard Poker game (Texas Hold'em), each player receives two cards, known as hole cards. There are also five community cards that are gradually placed on the table. The combination of the hole cards and community cards (Five cards in the aggregate) can form any of the following **hands**, ranging from a **Royal Flush**, the highest-ranking hand, to a **High Card**, the lowest.

A standard deck of cards consists of 52 cards, divided into four suits: Spades ♠, Hearts ♥, Clubs ♣, and Diamonds ♦. Each card in a suit has a rank, ranging from 2 to 14, with 14 representing the Ace. The ranks 11, 12, 13, and 14 are represented by J (Jack), Q (Queen), K (King), and A (Ace), respectively.

## Hands

- **ROYAL FLUSH**
  - A♠ K♠ Q♠ J♠ 10♠
- **STRAIGHT FLUSH**
  - 8♠ 7♠ 6♠ 5♠ 4♠
- **4 OF A KIND**
  - 5♠ 5♥ 5♣ 5♦ 3♥
- **FULL HOUSE**
  - K♥ K♦ K♠ 5♥ 5♣
- **FLUSH**
  - K♠ J♠ 9♠ 7♠ 3♠
- **STRAIGHT**
  - Q♠ J♦ 10♣ 9♠ 8♥
- **3 OF A KIND**
  - Q♠ Q♥ Q♦ 5♠ 9♣
- **2 PAIRS**
  - K♥ K♠ J♣ J♦ 9♦
- **PAIR**
  - A♣ A♦ 9♥ 6♠ 4♦
- **HIGH CARD**
  - A♦ 7♥ 5♣ 3♦ 2♠

# How does the hands rank detector work?

The Hand Rank Detector is a simple script developed in the C programming language. Its main function takes a string (an array of characters) representing the five community cards and two hole cards. The string is always 21 characters long and can be divided into seven groups, each representing a card. For example: 03D08D13H06H10D02D12C.

- S => Spades
- H => Hearts
- C => Clubs
- D => Diamonds
- 11 => Jack
- 12 => Queen
- 13 => King
- 14 => Ace

The script first validates the card combination. If the combination is valid, it attempts to find a matching hand in a top-down approach, returning the first match it finds.

The **generate_random_combinations** function generates random hands, which are then passed to **calculate_poker_rank**. You can also directly input your own custom combination into this function to see the results.

# How to run the app

- Open your console
- Build the app using **gcc -o main main.c**
- Run ./main
- Enjoy!
