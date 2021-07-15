#include "NetMsgCreateObject.h"
#ifdef AFTR_CONFIG_USE_BOOST

#include <iostream>
#include <sstream>
#include <string>
#include "AftrManagers.h"
#include "Vector.h"
#include "WO.h"
#include "GLView.h"
#include "WorldContainer.h"
#include "NetMsgCreateWOReply.h"
#include <utility>
#include "GLViewBattleship.h"

using namespace Aftr;

NetMsgMacroDefinition(NetMsgCreateObject);

NetMsgCreateObject::NetMsgCreateObject()
{

	this->Positon = "";

}

NetMsgCreateObject::~NetMsgCreateObject()
{
}

bool NetMsgCreateObject::toStream(NetMessengerStreamBuffer& os) const
{

	os << this->Positon;
	return true;
}

bool NetMsgCreateObject::fromStream(NetMessengerStreamBuffer& is)
{

	is >> this->Positon;
	return true;
}

void NetMsgCreateObject::onMessageArrived()
{
	//std::cout << "Message has arrived... " << this->toString() << "\n";

	//std::cout << "Payload\n";

	//std::cout << id << "/ ";

	std::cout << Positon << ".\n";

	std::string miss(ManagerEnvironmentConfiguration::getSMM() + "../../modules/Battleship/mm/models/miss.stl");
	std::string hit(ManagerEnvironmentConfiguration::getSMM() + "../../modules/Battleship/mm/models/hit.stl");

	//strings for mapping
	std::string A = "A";
	std::string B = "B";
	std::string C = "C";
	std::string D = "D";
	std::string E = "E";
	std::string F = "F";
	std::string G = "G";
	std::string H = "H";
	std::string I = "I";
	std::string J = "J";
	std::string K = "K";
	std::string L = "L";
	std::string M = "M";
	//push all strings into a vector
	std::vector<std::string> stringVec;
	stringVec.push_back(A);
	stringVec.push_back(B);
	stringVec.push_back(C);
	stringVec.push_back(D);
	stringVec.push_back(E);
	stringVec.push_back(F);
	stringVec.push_back(G);
	stringVec.push_back(H);
	stringVec.push_back(I);
	stringVec.push_back(J);
	stringVec.push_back(K);
	stringVec.push_back(L);
	stringVec.push_back(M);
	//WO* wo = ManagerGLView::getGLView()->getWorldContainer()->getWOByID(id);
	Aftr::WorldContainer* World = ManagerGLView::getGLView()->getWorldContainer();//get the worldLst
	static std::vector<std::pair<std::string, std::pair<float, float> >> Map;//create the map

	//starting coordinates for the mapping
	float x_pos_guess = 34.7;
	float y_pos_guess = 21.3;


	std::stringstream ss(Positon);//take the send payload as a string stream
	std::string MsgType;//var for determining the msg type
	ss >> MsgType;
	std::stringstream ssBoatPos(ManagerGLView::getGLView<GLViewBattleship>()->EnemyBoatPos);

	if (MsgType == "SETUP") {//if setup was the first string send set the doneSetup var to 1 to know the other player is done with setup
		ManagerGLView::getGLView<GLViewBattleship>()->doneSetup = 1;
		std::string tmpPosition;
		for (int i = 0; i < 17; i++) {//take the rest of the payload which is all the positions of the other players boats
			ss >> tmpPosition;
			ManagerGLView::getGLView<GLViewBattleship>()->EnemyBoatPos = ManagerGLView::getGLView<GLViewBattleship>()->EnemyBoatPos + " " + tmpPosition;
			tmpPosition = "";
		}

	}

	if (MsgType == "SHOT") {//if msg was a guess then it will have SHOT as the first string
		static bool setMap = false;
		bool missShot = true;//for knowing which WO to place down
		ManagerGLView::getGLView<GLViewBattleship>()->myTurn = true;//set players turn to true because opponenets turn is over after the guess
		if (setMap != true) {//create the mapping of the top
			for (int i = 0; i < 13; i++) {
				y_pos_guess = 21.3;
				for (int j = 0; j < 13; j++) {
					//std::cout << "\ny_pos_guess= " << y_pos_guess;
					std::string tmp = stringVec[j] + std::to_string(i + 1);
					std::pair<float, float> tempPair(y_pos_guess, x_pos_guess);
					std::pair<std::string, std::pair<float, float>> StringCordPair(tmp, tempPair);
					Map.push_back(StringCordPair);
					y_pos_guess = y_pos_guess - 1;
				}
				x_pos_guess = x_pos_guess - 1;
			}
			setMap = true;
		}

		std::string guess;
		ss >> guess;//next string will be the tile guessed
		std::cout << "\nguess made was: " << guess;
		std::string HitORMiss;
		ss >> HitORMiss;
		if (HitORMiss == "MISS") {//next string will be hit or miss depending on the guess type
			missShot = true;
		}
		if (HitORMiss == "HIT") {
			missShot = false;
		}
		
		std::pair<float, float> tmpPair;//set the coordinate
		for (int i = 0; i < Map.size(); i++) {
			if (Map[i].first == guess) {
				tmpPair = Map[i].second;
			}

		}

		if (missShot == true) {//create the WO miss
		//std::cout << "\nbottom coords are x= " << tmpPair.second << " y= " << tmpPair.first;
		WO* wo = WO::New(miss, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
		wo->setPosition(Vector(tmpPair.second, tmpPair.first, 4.2));//for top
		wo->rotateAboutGlobalX(-1.5708);
		wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
		wo->setLabel("missrec");
		World->push_back(wo);
		}
		else if (missShot == false) {//create the wo hit
			WO* wo = WO::New(hit, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
			wo->setPosition(Vector(tmpPair.second, tmpPair.first, 4.8));//for top
			wo->rotateAboutGlobalX(-1.5708);
			wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
			wo->setLabel("hitrec");
			World->push_back(wo);
		}
	}
	
}

std::string NetMsgCreateObject::toString() const
{
	std::stringstream ss;

	ss << NetMsg::toString();
	//ss << "Payload\n";
	//ss << id << "/ ";
	//ss << Positon << ".\n";


	return ss.str();
}

#endif