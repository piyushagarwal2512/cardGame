#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class Card {
  public:
  int index;
  int number;
  string display;
  string suit;

  Card(int index) {
    this->index = index;
    this->number = (index % 13)+1;
    if(index<13) {
      this->suit = "Spade";
    } else if(index<26) {
      this->suit = "Clubs";
    } else if(index<39) {
      this->suit = "Hearts";
    } else {
      this->suit = "Diamonds";
    }
    this->display = to_string(this->number);
    if(this->number==11) {
      this->display = "J";
    }
    if(this->number==12) {
      this->display = "Q";
    }
    if(this->number==13) {
      this->display = "K";
    }
    if(this->number==1) {
      this->display = "A";
    }
  }
};

class Player {
  public:
  int playerNumber;
  vector<Card> playerCards;

  Player(int i) {
    playerNumber = i;
  }

  void addCard(int cardIndex) {
    Card c = Card(cardIndex);
    playerCards.push_back(c);
  }
};

vector<Player> distributeCards(int players) {
  vector<int> deck(52, 0);
  for(int i=0;i<52;i++) {
    deck[i] = i;
  }
  srand(time(0));
  random_shuffle(deck.begin(), deck.end());

  int cardCount = 3;
  int k=0;
  vector<Player> distributedCards;

  for(int i=0; i<players;i++) {
    Player player(i+1);
    while(player.playerCards.size() != cardCount) {
      player.addCard(deck[k]);
      k++;
    }
    distributedCards.push_back(player);
  }
  return distributedCards;
}

int isTrail(vector<Card> cards) {
  bool trail = true;
  int n = 0;
  for(int i=0;i<cards.size();i++) {
    if(n==0) {
      n=cards[i].number;
    } else if(cards[i].number!=n) {
      return 0;
    }
  }
  return n;
}

int isSequence(vector<Card> cards) {
  int minimum = min(min(cards[0].number, cards[1].number), cards[2].number);
  int maximum = max(max(cards[0].number, cards[1].number), cards[2].number);
  if(maximum-minimum!=2) {
    return 0;
  }
  if(cards[0].number == minimum+1 || cards[1].number == minimum+1 || cards[2].number == minimum+1) {
    return maximum;
  }
  return 0;
}

pair<int,int> isPair(vector<Card> cards) {
  bool pair = true;
  if(cards[0].number==cards[1].number) {
    return make_pair(cards[0].number,cards[2].number);
  }
  if(cards[1].number==cards[2].number) {
    return make_pair(cards[1].number,cards[0].number);
  }
  if(cards[0].number==cards[2].number) {
    return make_pair(cards[0].number,cards[1].number);
  }
  return make_pair(0,0);
}

vector<Player> getWinner(vector<Player> players) {
  vector<Player> winners;
  if(isTrail(players[0].playerCards) || isTrail(players[1].playerCards) || isTrail(players[2].playerCards) || isTrail(players[3].playerCards)) {
    int winner;
    int maxScore=0;
    for(int i=0;i<players.size();i++) {
      int score = isTrail(players[i].playerCards);
      if(score>maxScore) {
        maxScore = score;
        winner = i;
      }
    }
    winners.push_back(players[winner]);
    return winners;
  }

  if(isSequence(players[0].playerCards) || isSequence(players[1].playerCards) || isSequence(players[2].playerCards) || isSequence(players[3].playerCards)) {
    int winner;
    int maxScore=0;
    for(int i=0;i<players.size();i++) {
      int score = isSequence(players[i].playerCards);
      if(score>maxScore) {
        maxScore = score;
        winner = i;
      }
    }
    winners.push_back(players[winner]);
    return winners;
  }

  if(isPair(players[0].playerCards).first || isPair(players[1].playerCards).first || isPair(players[2].playerCards).first || isPair(players[3].playerCards).first) {
    int winner;
    int maxScore=0;
    int highCard=0;
    for(int i=0;i<players.size();i++) {
      int score = isPair(players[i].playerCards).first;
      int hc = isPair(players[i].playerCards).second;
      if(maxScore == 0) {
        maxScore = score;
        highCard = hc;
        winner = i;
      } else if(score>maxScore) {
        maxScore = score;
        highCard = hc;
        winner = i;
      } else if(score == maxScore) {
        if(hc>highCard) {
          maxScore = score;
          highCard = hc;
          winner = i;
        }
      }
    }
    winners.push_back(players[winner]);
    return winners;
  }

  vector<int> topCards(4,0);
  int topCard = 0;
  for(int i=0;i<4;i++) {
    for(int j=0;j<3;j++) {
      if(players[i].playerCards[j].number == 1) {
        topCards[i] = 1;
        topCard = 1;
        break;
      } else {
        topCards[i] = max(topCards[i], players[i].playerCards[j].number);
        if(topCard != 1) {
          topCard = max(topCard, topCards[i]);
        }
      }
    } 
  }

  for(int i=0;i<4;i++) {
    if(topCards[i] == topCard) {
      winners.push_back(players[i]);
    }
  }

  return winners;
}

void showPlayerCards(vector<Player> players) {
  for(int i=0;i<players.size();i++) {
    cout<<"Player "<<players[i].playerNumber<<" got:"<<endl;
    for(int j=0;j<players[i].playerCards.size();j++) {
      cout<<players[i].playerCards[j].display<<" of "<<players[i].playerCards[j].suit<<endl;
    }
    cout<<endl;
  }
}

int getCard(unordered_set<int>& alreadyPicked) {
  while(true) {
    int newCard = rand() % 52 + 1;
    if(alreadyPicked.find(newCard) == alreadyPicked.end()) {
      alreadyPicked.insert(newCard);
      return newCard;
    }
  }
}

Player resolveDraw(vector<Player> players, unordered_set<int>& alreadyPicked) {
  cout<<"Draw between Player "<<players[0].playerNumber;
  for(int i=1;i<players.size();i++) {
    cout<<" and Player "<<players[1].playerNumber;
  }
  cout<<endl;
  int topCard = 0;
  vector<int> newCards(players.size(), 0);
  for(int i=0;i<players.size();i++) {
    int c = getCard(alreadyPicked);
    Card card = Card(c);
    newCards[i] = card.number;
    topCard = max(topCard, newCards[i]);
    if(topCard==1) {
      break;
    }
    cout<<"Player "<<players[i].playerNumber<<" picked "<<card.display<<" of "<<card.suit<<endl;
  }

  int winner = -1;
  for(int i=0;i<players.size();i++) {
    if(newCards[i]==topCard) {
      if(winner == -1) {
        winner = i;
      } else {
        return resolveDraw(players, alreadyPicked);
      }
    }
  }
  return players[winner];
}

int main() {
	
  int num_of_players = 4;
  vector<Player> players = distributeCards(num_of_players);

  showPlayerCards(players);

  vector<Player> winners = getWinner(players);
  
  unordered_set<int> alreadyPicked;
  for(int i=0;i<players.size();i++) {
    for(int j=0;j<3;j++) {
      alreadyPicked.insert(players[i].playerCards[j].index);
    } 
  }

  Player winner = winners[0];
  if(winners.size()>1) {
    winner = resolveDraw(winners, alreadyPicked);
  }

  cout<<"Winner is player "<<winner.playerNumber;

	return 0;
}
