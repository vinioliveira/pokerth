/***************************************************************************
 *   Copyright (C) 2006 by FThauer FHammer   *
 *   f.thauer@web.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "localhand.h"
#include "tools.h"
#include "cardsvalue.h"
#include <game_defs.h>

#include <iostream>

using namespace std;

LocalHand::LocalHand(boost::shared_ptr<EngineFactory> f, GuiInterface *g, BoardInterface *b, std::vector<boost::shared_ptr<PlayerInterface> > sl_old, PlayerList sl, PlayerList apl, PlayerList rpl, int id, int sP, unsigned dP, int sB,int sC)
: myFactory(f), myGui(g),  myBoard(b), playerArray(sl_old), seatsList(sl), activePlayerList(apl), runningPlayerList(rpl), myBeRo(0), myID(id), startQuantityPlayers(sP), dealerPosition(dP), actualRound(0), smallBlind(sB), startCash(sC), lastPlayersTurn(0), allInCondition(false),
  cardsShown(false), bettingRoundsPlayed(0)
{

	int i, j, k;
	PlayerListIterator it;

	CardsValue myCardsValue;

	myBoard->setHand(this);

// 	for(i=0; i<startQuantityPlayers; i++) {
// 		playerArray[i]->setHand(this);
// 	// myFlipCards auf 0 setzen
// 		playerArray[i]->setMyCardsFlip(0, 0);
// 	}

	for(it=seatsList->begin(); it!=seatsList->end(); it++) {
		(*it)->setHand(this);
	// myFlipCards auf 0 setzen
		(*it)->setMyCardsFlip(0, 0);
	}


	// roundStartCashArray fllen
// 	for(i=0; i<MAX_NUMBER_OF_PLAYERS; i++) {
// 		playerArray[i]->setMyRoundStartCash(playerArray[i]->getMyCash());
// 	}
	
	// Karten generieren und Board sowie Player zuweisen
	int *cardsArray = new int[2*activePlayerList->size()+5];
	Tools::getRandNumber(0, 51, 2*activePlayerList->size()+5, cardsArray, 1);
	int tempBoardArray[5];
	int tempPlayerArray[2];
	int tempPlayerAndBoardArray[7];
	int bestHandPos[5];
	int sBluff;
	for(i=0; i<5; i++) {
		tempBoardArray[i] = cardsArray[i];
		tempPlayerAndBoardArray[i+2] = cardsArray[i];
	}

	k = 0;
	myBoard->setMyCards(tempBoardArray);
	for(it=activePlayerList->begin(); it!=activePlayerList->end(); it++, k++) {

		(*it)->getMyBestHandPosition(bestHandPos);

		for(j=0; j<2; j++) {
			tempPlayerArray[j] = cardsArray[2*k+j+5];
			tempPlayerAndBoardArray[j] = cardsArray[2*k+j+5];
		}

		(*it)->setMyCards(tempPlayerArray);
		(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray, bestHandPos));
		(*it)->setMyBestHandPosition(bestHandPos);
		(*it)->setMyRoundStartCash((*it)->getMyCash());

		// myBestHandPosition auf Fehler ueberpruefen
		for(j=0; j<5; j++) {
			if (bestHandPos[j] == -1) {
				cout << "ERROR getMyBestHandPosition in localhand.cpp" << endl;
			}
		}

		// sBluff für alle aktiver Spieler außer human player setzen --> TODO for ai-player in internet
		if((*it)->getMyID() != 0) {
			Tools::getRandNumber(1,100,1,&sBluff,0);
			(*it)->setSBluff(sBluff);
			(*it)->setSBluffStatus(0);
		}
	}

	delete[] cardsArray;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!   DEBUGGER   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //

	if(DEBUG_MODE) {

		int temp5Array[5];

		switch(myID) {

			case 1: {		

				tempBoardArray[0] = 27;
				tempBoardArray[1] = 37;
				tempBoardArray[2] = 45;
				tempBoardArray[3] = 51;
				tempBoardArray[4] = 23;
			
				myBoard->setMyCards(tempBoardArray);
			
				tempPlayerAndBoardArray[2] = tempBoardArray[0];
				tempPlayerAndBoardArray[3] = tempBoardArray[1];
				tempPlayerAndBoardArray[4] = tempBoardArray[2];
				tempPlayerAndBoardArray[5] = tempBoardArray[3];
				tempPlayerAndBoardArray[6] = tempBoardArray[4];
		
				// player0
				it = seatsList->begin();
			
// 				tempPlayerArray[0] = 51;
// 				tempPlayerArray[1] = 20;
// 				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
// 				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
// 			
// 				(*it)->setMyCards(tempPlayerArray);
// 				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
// 			
// 				(*it)->setMyBestHandPosition(temp5Array);

				// player1
				it++;
		
				tempPlayerArray[0] = 12;
				tempPlayerArray[1] = 48;
				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
			
				(*it)->setMyCards(tempPlayerArray);
				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
			
				(*it)->setMyBestHandPosition(temp5Array);
			
				// player2
				it++;
		
// 				tempPlayerArray[0] = 25;
// 				tempPlayerArray[1] = 36;
// 				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
// 				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
// 			
// 				(*it)->setMyCards(tempPlayerArray);
// 				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
// 			
// 				(*it)->setMyBestHandPosition(temp5Array);
	
				// player3
				it++;
			
// 				tempPlayerArray[0] = 12;
// 				tempPlayerArray[1] = 46;
// 				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
// 				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
// 			
// 				(*it)->setMyCards(tempPlayerArray);
// 				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
// 			
// 				(*it)->setMyBestHandPosition(temp5Array);

				// player4
				it++;
			
				tempPlayerArray[0] = 25;
				tempPlayerArray[1] = 16;
				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
			
				(*it)->setMyCards(tempPlayerArray);
				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
			
				(*it)->setMyBestHandPosition(temp5Array);

				// player5
				it++;
			
				tempPlayerArray[0] = 43;
				tempPlayerArray[1] = 30;
				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
			
				(*it)->setMyCards(tempPlayerArray);
				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
			
				(*it)->setMyBestHandPosition(temp5Array);


			} break;
			case 2: {
		
/*				tempBoardArray[0] = 48;
				tempBoardArray[1] = 21;
				tempBoardArray[2] = 4;
				tempBoardArray[3] = 9;
				tempBoardArray[4] = 0;
			
				myBoard->setMyCards(tempBoardArray);
			
				tempPlayerAndBoardArray[2] = tempBoardArray[0];
				tempPlayerAndBoardArray[3] = tempBoardArray[1];
				tempPlayerAndBoardArray[4] = tempBoardArray[2];
				tempPlayerAndBoardArray[5] = tempBoardArray[3];
				tempPlayerAndBoardArray[6] = tempBoardArray[4];*/
		
				// player0
				it = seatsList->begin();
			
// 				tempPlayerArray[0] = 9;
// 				tempPlayerArray[1] = 5;
// 				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
// 				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
// 			
// 				(*it)->setMyCards(tempPlayerArray);
// 				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
// 			
// 				(*it)->setMyBestHandPosition(temp5Array);

				// player1
				it++;
		
// 				tempPlayerArray[0] = 47;
// 				tempPlayerArray[1] = 22;
// 				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
// 				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
// 			
// 				(*it)->setMyCards(tempPlayerArray);
// 				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
// 			
// 				(*it)->setMyBestHandPosition(temp5Array);
			
				// player2
				it++;
		
// 				tempPlayerArray[0] = 33;
// 				tempPlayerArray[1] = 27;
// 				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
// 				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
// 			
// 				(*it)->setMyCards(tempPlayerArray);
// 				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
// 			
// 				(*it)->setMyBestHandPosition(temp5Array);
	
				// player3
				it++;
			
// 				tempPlayerArray[0] = 38;
// 				tempPlayerArray[1] = 22;
// 				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
// 				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
// 			
// 				(*it)->setMyCards(tempPlayerArray);
// 				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
// 			
// 				(*it)->setMyBestHandPosition(temp5Array);

				// player4
				it++;
			
// 				tempPlayerArray[0] = 25;
// 				tempPlayerArray[1] = 16;
// 				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
// 				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
// 			
// 				(*it)->setMyCards(tempPlayerArray);
// 				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
// 			
// 				(*it)->setMyBestHandPosition(temp5Array);

				// player5
				it++;
			
// 				tempPlayerArray[0] = 27;
// 				tempPlayerArray[1] = 10;
// 				tempPlayerAndBoardArray[0] = tempPlayerArray[0];
// 				tempPlayerAndBoardArray[1] = tempPlayerArray[1];
// 			
// 				(*it)->setMyCards(tempPlayerArray);
// 				(*it)->setMyCardsValueInt(myCardsValue.cardsValue(tempPlayerAndBoardArray,temp5Array));
// 			
// 				(*it)->setMyBestHandPosition(temp5Array);

			} break;
			default: {}

		}



	}

// ----------------------------------------

	// Dealer, SB, BB bestimmen
	assignButtons();

	myBeRo = myFactory->createBeRo(this, myID, dealerPosition, smallBlind);
}



LocalHand::~LocalHand()
{
}

void LocalHand::start() {

	/////////////////////

	// Karten austeilen
	myGui->dealHoleCards();

	getBoard()->collectSets();
	getGuiInterface()->refreshPot();
	
	//Rundenwechsel | beim ersten Durchlauf --> Preflop starten
	myGui->nextPlayerAnimation();
}

void LocalHand::assignButtons() {

	size_t i;
	PlayerListIterator it;
	PlayerListConstIterator it_c;

	// alle Buttons loeschen
// 	for (i=0; i<MAX_NUMBER_OF_PLAYERS; i++) { playerArray[i]->setMyButton(0); }

	// alle Buttons loeschen
	for (it=seatsList->begin(); it!=seatsList->end(); it++) {
		(*it)->setMyButton(BUTTON_NONE);
	}

	// DealerButton zuweisen
// 	playerArray[dealerPosition]->setMyButton(1);

	// DealerButton zuweisen
	it = getSeatIt(dealerPosition);
	assert(it != seatsList->end() );
	(*it)->setMyButton(BUTTON_DEALER);


	// assign Small Blind next to dealer. ATTENTION: in heads up it is big blind
// 	i = dealerPosition;
// 
// 	for(j=0; (j<MAX_NUMBER_OF_PLAYERS && !(playerArray[i]->getMyActiveStatus())) || j==0; j++) {
// 
// 		i = (i+1)%(MAX_NUMBER_OF_PLAYERS);
// 		if(playerArray[i]->getMyActiveStatus())	{
// 			if(activePlayerList->size() > 2) playerArray[i]->setMyButton(2); //small blind normal
// 			else playerArray[i]->setMyButton(3); //big blind in heads up		
// 		}
// 	}


	// assign big blind next to small blind. ATTENTION: in heads up it is small blind
// 	for(j=0; (j<MAX_NUMBER_OF_PLAYERS && !(playerArray[i]->getMyActiveStatus())) || j==0; j++) {
// 
// 		i = (i+1)%(MAX_NUMBER_OF_PLAYERS);
// 		if(playerArray[i]->getMyActiveStatus())	{
// 			if(activePlayerList->size() > 2) playerArray[i]->setMyButton(3); //big blind normal
// 			else playerArray[i]->setMyButton(2); //small blind in heads up
// 			
// 		}
// 	}



	// assign Small Blind next to dealer. ATTENTION: in heads up it is big blind
	// assign big blind next to small blind. ATTENTION: in heads up it is small blind
	bool nextActivePlayerFound = false;
	PlayerListIterator dealerPositionIt = getSeatIt(dealerPosition);
	assert( dealerPositionIt != seatsList->end() );

	for(i=0; i<seatsList->size(); i++) {

		dealerPositionIt++;
		if(dealerPositionIt == seatsList->end()) dealerPositionIt = seatsList->begin();

		it = getActivePlayerIt( (*dealerPositionIt)->getMyUniqueID() );
		if(it != activePlayerList->end() ) {
			nextActivePlayerFound = true;
			if(activePlayerList->size() > 2) (*it)->setMyButton(2); //small blind normal
			else (*it)->setMyButton(3); //big blind in heads up

			it++;
			if(it == activePlayerList->end()) it = activePlayerList->begin();

			if(activePlayerList->size() > 2) (*it)->setMyButton(3); //big blind normal
			else (*it)->setMyButton(2); //small blind in heads up

			break;
		}

	}
	assert(nextActivePlayerFound);



















	//do sets --> TODO switch?
	for (it_c=runningPlayerList->begin(); it_c!=runningPlayerList->end(); it_c++) { 

		//small blind
		if((*it_c)->getMyButton() == BUTTON_SMALL_BLIND) { 

			// All in ?
			if((*it_c)->getMyCash() <= smallBlind) {

				(*it_c)->setMySet((*it_c)->getMyCash());
				// 1 to do not log this
				(*it_c)->setMyAction(PLAYER_ACTION_ALLIN,1);
				// delete this player from runningPlayerList
// 				it_c = runningPlayerList->erase(it_c);

			}
			else {
				(*it_c)->setMySet(smallBlind);
			}
		}

		//big blind
		if((*it_c)->getMyButton() == BUTTON_BIG_BLIND) { 
	
			// all in ?
			if((*it_c)->getMyCash() <= 2*smallBlind) {

				(*it_c)->setMySet((*it_c)->getMyCash());
				// 1 to do not log this
				(*it_c)->setMyAction(PLAYER_ACTION_ALLIN,1);
				// delete this player from runningPlayerList
// 				it_c = runningPlayerList->erase(it_c);
	
			}
			else {
				(*it_c)->setMySet(2*smallBlind);
			}
		}
	}
}


void LocalHand::switchRounds() {

// 	cout << "playerID begin switchRounds(): " << getCurrentBeRo()->get

	PlayerListIterator it, it_1;
	PlayerListConstIterator it_c;

	// refresh runningPlayerList
	for(it=runningPlayerList->begin(); it!=runningPlayerList->end(); ) {
		if((*it)->getMyAction() == PLAYER_ACTION_FOLD || (*it)->getMyAction() == PLAYER_ACTION_ALLIN) {
			it = runningPlayerList->erase(it);
			if(!(runningPlayerList->empty())) {

				it_1 = it;
				if(it_1 == runningPlayerList->begin()) it_1 = runningPlayerList->end();
				it_1--;
				getCurrentBeRo()->setCurrentPlayersTurnId((*it_1)->getMyUniqueID());

			}
		} else {
			it++;
		}
	}

	// Anzahl der Spieler ermitteln, welche All In sind
	int allInPlayersCounter = 0;
	for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); it_c++) {
		if ((*it_c)->getMyAction() == PLAYER_ACTION_ALLIN) allInPlayersCounter++;
	}

// 	cout << "allInPlayersCounter: " << allInPlayersCounter << endl;

	// TODO -> runningPlayerList.size()
	int nonFoldPlayerCounter = 0;
	for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); it_c++) {
		if ((*it_c)->getMyAction() != PLAYER_ACTION_FOLD) nonFoldPlayerCounter++;
	}

// 	cout << "nonFoldPlayerCounter: " << nonFoldPlayerCounter << endl;

	//wenn nur noch einer nicht-folded dann gleich den Pot verteilen
	if(nonFoldPlayerCounter==1) {
		myBoard->collectPot();	
		myGui->refreshPot();
		myGui->refreshSet();
		actualRound = 4; 
	}
	// prfen der All In Kondition
	// fr All In Prozedur mssen mindestens zwei aktive Player vorhanden sein
	else {

		// 1) wenn alle All In
		if(allInPlayersCounter == nonFoldPlayerCounter) {
			allInCondition = true;
		}

		// 2) alle bis auf einen All In und der hat HighestSet
		if(allInPlayersCounter+1 == nonFoldPlayerCounter) {

			// TODO -> runningPlayerList s.u.
// 			for(i=0; i<MAX_NUMBER_OF_PLAYERS; i++) {
// 
// 				if(playerArray[i]->getMyAction() != 1 && playerArray[i]->getMyAction() != 6 && playerArray[i]->getMyActiveStatus() == 1 && playerArray[i]->getMySet() >= myBeRo[actualRound]->getHighestSet()) {
// 						allInCondition = 1;
// 				}
// 
// 			}


			for(it_c=runningPlayerList->begin(); it_c!=runningPlayerList->end(); it_c++) {

				if((*it_c)->getMySet() >= myBeRo[actualRound]->getHighestSet()) {
						allInCondition = true;
				}

			}

// 			if( (*(runningPlayerList.begin()))->getMySet() >= myBeRo[actualRound]->getHighestSet() ) {
// 				allInCondition = 1;
// 			}

// 			// HeadsUp-Ausnahme -> spieler ermitteln, der all in ist und als bb nur weniger als sb setzen konnte
// 			for(i=0; i<MAX_NUMBER_OF_PLAYERS; i++) {
// 
// 				if(activePlayerList->size()==2 && playerArray[i]->getMyAction() == 6 && playerArray[i]->getMyActiveStatus() == 1 && playerArray[i]->getMyButton()==3 && playerArray[i]->getMySet()<=smallBlind) {
// 						allInCondition = 1;
// 				}
// 
// 			}



			// exception
				// no.1: if in first Preflop Round next player is small blind and only all-in-big-blind with less than smallblind amount and other all-in players with less than small blind are nonfold too -> preflop is over
			PlayerListConstIterator smallBlindIt_c = getRunningPlayerIt(myBeRo[actualRound]->getSmallBlindPositionId());
			PlayerListConstIterator bigBlindIt_c = getActivePlayerIt(myBeRo[actualRound]->getBigBlindPositionId());
			if(smallBlindIt_c!=runningPlayerList->end() &&  bigBlindIt_c!=activePlayerList->end() && actualRound == GAME_STATE_PREFLOP && myBeRo[actualRound]->getFirstRound()) {
				// determine player who are all in with less than small blind amount
				int tempCounter = 0;
				for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); it_c++) {
					if((*it_c)->getMyAction() == PLAYER_ACTION_ALLIN && (*it_c)->getMySet() <= smallBlind) {
						tempCounter++;
					}
				}
				if( (*bigBlindIt_c)->getMySet() <= smallBlind  && tempCounter == allInPlayersCounter) { 
					allInCondition = true;
				}
			}




			// HeadsUp-Ausnahme -> spieler ermitteln, der all in ist und als bb nur weniger als sb setzen konnte
			for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); it_c++) {

				if(activePlayerList->size()==2 && (*it_c)->getMyAction() == PLAYER_ACTION_ALLIN && (*it_c)->getMyButton()==BUTTON_BIG_BLIND && (*it_c)->getMySet()<=smallBlind) {
						allInCondition = true;
				}

			}

		}
	}

	// beim Vorliegen einer All In Kondition -> Ausfhrung einer Sonderprozedur
	if(allInCondition) {
		myBoard->collectPot();	
		myGui->refreshPot();
		myGui->refreshSet();
		myGui->flipHolecardsAllIn();
		
		if (actualRound < 4) // do not increment past 4
			actualRound++;
		
		//log board cards for allin
		if(actualRound >= 1) {
			int tempBoardCardsArray[5];
			
			myBoard->getMyCards(tempBoardCardsArray);
			myGui->logDealBoardCardsMsg(actualRound, tempBoardCardsArray[0], tempBoardCardsArray[1], tempBoardCardsArray[2], tempBoardCardsArray[3], tempBoardCardsArray[4]);
		}
	
	}

	//unhighlight current players groupbox
// 	if(playerArray[lastPlayersTurn]->getMyActiveStatus() == 1) myGui->refreshGroupbox(lastPlayersTurn,1);




	//unhighlight current players groupbox
	it_c = getActivePlayerIt(lastPlayersTurn);
	if( it_c != activePlayerList->end() ) { 
		// lastPlayersTurn is active
		myGui->refreshGroupbox(lastPlayersTurn,1);
	}


	myGui->refreshGameLabels((GameState)getActualRound());
// 	/*/*/*/*cout <<*/*/*/*/ "NextPlayerSpeed1 stop" << endl;
// 


// 	cout << "NextPlayerSpeed2 start" << endl;
	switch(actualRound) {
		case 0: {
			// Preflop starten
			myGui->preflopAnimation1();
		} break;
		case 1: {
			// Flop starten
			myGui->flopAnimation1();
		} break;
		case 2: {
			// Turn starten
			myGui->turnAnimation1();

		} break;
		case 3: {
			// River starten
			myGui->riverAnimation1();

		} break;
		case 4: {
			// PostRiver starten
			myGui->postRiverAnimation1();

		} break;
		default: {}

	

	}

}

PlayerListIterator LocalHand::getSeatIt(unsigned uniqueId) const {

	PlayerListIterator it;

	for(it=seatsList->begin(); it!=seatsList->end(); it++) {
		if((*it)->getMyUniqueID() == uniqueId) {
			break;
		}
	}

	return it;

}

PlayerListIterator LocalHand::getActivePlayerIt(unsigned uniqueId) const {

	PlayerListIterator it;

	for(it=activePlayerList->begin(); it!=activePlayerList->end(); it++) {
		if((*it)->getMyUniqueID() == uniqueId) {
			break;
		}
	}

	return it;

}

PlayerListIterator LocalHand::getRunningPlayerIt(unsigned uniqueId) const {

	PlayerListIterator it;

	for(it=runningPlayerList->begin(); it!=runningPlayerList->end(); it++) {

// 		cout << (*it)->getMyUniqueID() << " ";

		if((*it)->getMyUniqueID() == uniqueId) {
			break;
		}
	}

// 	cout << endl;

	return it;

}
