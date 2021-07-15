#include "GUIWO.h"
#ifdef AFTR_CONFIG_USE_IMGUI
#include "AftrImGuiIncludes.h"
#include "Mat4.h"


using namespace Aftr;

GuiWOclass* GuiWOclass::New(WOGUI* parentWOGUI, float width, float height) {
	GuiWOclass* imgui = new GuiWOclass(parentWOGUI);
	imgui->onCreate(width, height);
	return imgui;

}


GuiWOclass::GuiWOclass(WOGUI* parentWOGUI) : IFace(this), WOImGuiAbstract(parentWOGUI) {}

void GuiWOclass::onCreate(float width, float height) {
	this->client = NetMessengerClient::New("127.0.0.1", "12686");

	WOImGuiAbstract::onCreate(width, height);

}

GuiWOclass::~GuiWOclass() {}

void GuiWOclass::onUpdateWO() {}


void GuiWOclass::drawImGui_for_this_frame()
{
	//string vars for creating the maping of tiles
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


	std::vector<std::string> stringVec; 	//store them into a vector
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


	//bool vars for displaying windows in the gui
	static bool show_board = false;		//the guessing gui
	static bool test_move = false;		//the setup gui window check 
	static bool Done_With_setup = false;//checking if the setup phase is done

	//set position cords for the top board mapping
	float y_pos_guess = 21.3;
	float z_pos_guess = 15.8;

	//set position cords for the bottom board mapping
	float x_pos_bot = 34.7;
	float y_pos_bot = 21.3;

	

	static std::vector<std::pair<std::string, std::pair<float, float> >> Map;
	static std::vector<std::pair<std::string, std::pair<float, float> >> MapBottom;//map for the bottom board used for determining what tiles the boats are on
	std::vector<std::string> BoatRange;

	static int shipselect = 0;										//used in the menu when selecting different boats to move around
	static bool initInitPlacement = false;							//only here to make sure the following vector is only initlized once
	static std::vector<bool> initPlacement;							//vector to keep track of which boats have been placed if not all are placed before pressing done

	bool Turn = ManagerGLView::getGLView<GLViewBattleship>()->myTurn;//Turn variable keeps track if it is this players turn to guess
																	 //stored in the GLview becasuse it needs to be modified by netmsgs

	//push 5 false's into the placement vector one for each boat
	if (initInitPlacement == false) {
	initPlacement.push_back(false);
	initPlacement.push_back(false);
	initPlacement.push_back(false);
	initPlacement.push_back(false);
	initPlacement.push_back(false);
	initInitPlacement = true;					//set the init to true so it is only done once
	}
	ImGui::Begin("BattleShip");					// create window 

	ImGui::Text("Menu"); 

	static bool setMapBot = false;				//bool to ensure that the bottom mapping is only ran one time
	if (setMapBot != true) {
		for (int i = 0; i < 13; i++) {
			y_pos_bot = 21.3;					//starting y cordinate of each row
			for (int j = 0; j < 13; j++) {
				std::string tmp = stringVec[j] + std::to_string(i + 1);
				std::pair<float, float> tempPair(y_pos_bot, x_pos_bot);
				std::pair<std::string, std::pair<float, float>> StringCordPair(tmp, tempPair);
				MapBottom.push_back(StringCordPair);
				y_pos_bot = y_pos_bot - 1;
			}
			x_pos_bot = x_pos_bot - 1;
		}
		setMapBot = true;
	}

	
	ImGui::Separator();
	if (Turn == true) {					//if it is the player board show the check box to pull up the guessing window of the gui
	ImGui::Checkbox("Show Board", &show_board);
	}
	else if (Turn == false){			//if it is not the players turn close the window by setting this var to false
		show_board = false;
	}
	if (Done_With_setup == false) {		//if the setup phase is not complete show the checkbox to bring up the boat placement window
	ImGui::Checkbox("Setup", &test_move);
	}

	ImGui::End();						//end of the main menu window

	static bool setMap = false;			//set mapping for the top board only once
	if (setMap != true) {
		for (int i = 0; i < 13; i++) {
			y_pos_guess = 21.3;
			for (int j = 0; j < 13; j++) {
				std::cout << "\ny_pos_guess= " << y_pos_guess;
				std::string tmp = stringVec[j] + std::to_string(i + 1);
				std::pair<float, float> tempPair(y_pos_guess, z_pos_guess);
				std::pair<std::string, std::pair<float, float>> StringCordPair(tmp, tempPair);
				Map.push_back(StringCordPair);
				y_pos_guess = y_pos_guess - 1;
			}
			z_pos_guess = z_pos_guess - 1;
		}
		setMap = true;//mapping is set
	}

	if (show_board == true) {//if show board check box is pressed show the guessing window

	ImGui::Begin("Board");										// create window 
	for (int i = 0; i < 13; i++) {

		for (int j = 0; j < 13; j++) {
			std::string tmp = stringVec[j] + std::to_string(i+1);//loop through the stringVec and concatinate with the number it corrilates to
			const char* tmp2 = tmp.c_str();						//convert to c string for the button name
			if (ImGui::Button(tmp2, ImVec2(30, 30)))
				handleGuess(tmp, Map);							//if button is pressed pass the handleGuess function the string of the tile guesses & the mapping of the top
			if (j != 12)
				ImGui::SameLine();

		}
	}

	ImGui::End();
	}

	if (test_move == true && Done_With_setup == false) {//if setup checkbox is pressed and the setup is not complete show the window to move around the pieces
		setCurrentWO(this->Ship_pieces[shipselect]);	//set the selected world object to the ship that is being selected
		Mat4 mat1 = this->object->getPose();
		static float x = mat1[12];						//grab the x pos
		static float y = mat1[13];						//grab the y pos

		
		ImGui::Separator();
		//menu for selecting the ship to place down
		if(ImGui::BeginMenu("Select piece")){
			if (ImGui::MenuItem("Carrier")) {
				shipselect = 0;
				setCurrentWO(this->Ship_pieces[shipselect]);
				if (initPlacement[shipselect] == false) {//the ship has not been placed yet move it to the board
					Mat4 mat = this->object->getPose();
					mat[12] = 27.7;
					x = mat[12];
					mat[13] = 15.3;
					y = mat[13];
					mat[14] = 3.8;
					this->object->setPose(mat);
					initPlacement[shipselect] = true;
				}

			}
			if (ImGui::MenuItem("Battleship")) {
				shipselect = 1;
				setCurrentWO(this->Ship_pieces[shipselect]);
				if (initPlacement[shipselect] == false) {//the ship has not been placed yet move it to the board
					Mat4 mat = this->object->getPose();
					mat[12] = 28.7;
					mat[13] = 15.8;
					mat[14] = 3.8;
					this->object->setPose(mat);
					initPlacement[shipselect] = true;

				}
			}
			if (ImGui::MenuItem("Cruiser")) {
				shipselect = 2;
				setCurrentWO(this->Ship_pieces[shipselect]);
				if (initPlacement[shipselect] == false) {//the ship has not been placed yet move it to the board
					Mat4 mat = this->object->getPose();
					mat[12] = 29.7;
					mat[13] = 15.3;
					mat[14] = 3.8;
					this->object->setPose(mat);
					initPlacement[shipselect] = true;

				}
			}
			if (ImGui::MenuItem("Submarine")) {
				shipselect = 3;
				setCurrentWO(this->Ship_pieces[shipselect]);
				if (initPlacement[shipselect] == false) {//the ship has not been placed yet move it to the board
					Mat4 mat = this->object->getPose();
					mat[12] = 30.7;
					mat[13] = 15.3;
					mat[14] = 3.8;
					this->object->setPose(mat);
					initPlacement[shipselect] = true;

				}
			}
			if (ImGui::MenuItem("Destroyer")) {
				shipselect = 4;
				setCurrentWO(this->Ship_pieces[shipselect]);
				if (initPlacement[shipselect] == false) {//the ship has not been placed yet move it to the board
					Mat4 mat = this->object->getPose();
					mat[12] = 31.7;
					mat[13] = 14.8;
					mat[14] = 3.8;
					this->object->setPose(mat);
					initPlacement[shipselect] = true;

				}
			}

		ImGui::EndMenu();
		}
		//debugging print statements for viewing the cordinates and look direction of each piece
		/*******************************************************************************
		ImGui::Separator();
		Mat4 mat = this->object->getPose();
		Aftr::Vector LookDir = this->object->getLookDirection();
		for (int i = 0; i < 16; i++) {
			float tmp = mat[i];
			if (i % 4 == 0)
				ImGui::Text(" %f ", tmp); //text box
			else {
				ImGui::SameLine();
				ImGui::Text(" %f ", tmp);
			}

		}
		ImGui::Separator();
		for (int j = 0; j < 3; j++) {
			ImGui::Text(" %f ", LookDir[j]); //text box
		
		}
		ImGui::Separator();
		*******************************************************************************/

		std::string boatLabel = this->object->getLabel();		//set the label of the WO that is being moved
		static std::vector<std::vector<std::string>> AllRanges;//vector for storeing the range of tiles all ships are currently on
		static bool initAllranges = false;						//variable to check that the previous vector is only initlized once
		if (initAllranges == false) {
		for (int i = 0; i < 5; i++) {
			AllRanges.push_back(BoatRange);					//push back 5 empty spaces to store all boat ranges
		}
		initAllranges = true;								//set this true so it doesnt push any more on to it
		}


		updateBoatPos(BoatRange, AllRanges, MapBottom);//updates the coordnates on the map for boat being moved

		//debugging code for checking the range of tiles each boat is on is working
		/**************************************************************************
		for (int j = 0; j < BoatRange.size(); j++) {
			std::string tempStr = BoatRange[j];
			const char* tempCSTR = tempStr.c_str();
			ImGui::Text(" %s ", tempCSTR);
			ImGui::SameLine();
		}
		
		ImGui::Separator();
		**************************************************************************/

		ImGui::Indent(90.0f);
		//*****************************
		//	Forward Button
		//*****************************
		if (ImGui::Button("Forward", ImVec2(80, 60))) {
			bool OffMap = false;						//var for detecting if the movement would be off of the map
			for (int i = 0; i < BoatRange.size(); i++) {//checks if any of the tiles the current boat is on is a top tile if so it changes the offMap var
				if (BoatRange[i] == "A1" || BoatRange[i] == "B1" || BoatRange[i] == "C1" || BoatRange[i] == "D1" || BoatRange[i] == "E1" || BoatRange[i] == "F1" || BoatRange[i] == "G1" || BoatRange[i] == "H1" || BoatRange[i] == "I1" || BoatRange[i] == "J1" || BoatRange[i] == "K1" || BoatRange[i] == "L1" || BoatRange[i] == "M1")
					OffMap = true;
			}
			if (OffMap == false) {						//if forward movement would not be off of the map move it forward one tile
				this->object->moveRelative(Aftr::Vector(1, 0, 0));
				mat1 = this->object->getPose();
				x = mat1[12];
				y = mat1[13];
			}
			updateBoatPos(BoatRange, AllRanges, MapBottom);//update the ranges
			int BoatIndex = 0;						//index for which boat is being moved
			bool Collision = false;					//variable for detecting if there was a collision
			for (int i = 0; i < BoatRange.size(); i++) {
				std::string TempCheckString = BoatRange[i];
				if (boatLabel == "boat5")
					BoatIndex = 0;
				if(boatLabel == "boat4")
					BoatIndex = 1;
				if(boatLabel == "boat3")
					BoatIndex = 2;
				if (boatLabel == "sub")
					BoatIndex = 3;
				if (boatLabel == "boat2")
					BoatIndex = 4;
				for (int j = 0; j < AllRanges.size(); j++) {//if any tiles in current boat range match that of any other boat there was a collision
					if (AllRanges[j].size() > 1 && j != BoatIndex) {
						for (int k = 0; k < AllRanges[j].size(); k++) {
							if (TempCheckString == AllRanges[j][k])
								Collision = true;
						}
					}
				}

			}
			if (Collision == true) {						//if there was a collision undo the movement
				this->object->moveRelative(Aftr::Vector(-1, 0, 0));
				mat1 = this->object->getPose();
				x = mat1[12];
				y = mat1[13];
			}

		}
		ImGui::Indent(-30.0f);
		//*****************************
		//	Left Button		(follows similar logic to forward)
		//*****************************
		if (ImGui::Button("Left", ImVec2(60, 60))) {
			bool OffMap = false;//var for detecting if the movement would be off of the map
			for (int i = 0; i < BoatRange.size(); i++) {//checks if any of the tiles the current boat is on is a left most tile if so it changes the offMap var
				if (BoatRange[i] == "A1" || BoatRange[i] == "A2" || BoatRange[i] == "A3" || BoatRange[i] == "A4" || BoatRange[i] == "A5" || BoatRange[i] == "A6" || BoatRange[i] == "A7" || BoatRange[i] == "A8" || BoatRange[i] == "A9" || BoatRange[i] == "A10" || BoatRange[i] == "A11" || BoatRange[i] == "A12" || BoatRange[i] == "A13")
					OffMap = true;
			}
			if (OffMap == false) {
				this->object->moveRelative(Aftr::Vector(0, 1, 0));
				mat1 = this->object->getPose();
				x = mat1[12];
				y = mat1[13];
			}
			updateBoatPos(BoatRange, AllRanges, MapBottom);
			int BoatIndex = 0;
			bool Collision = false;
			for (int i = 0; i < BoatRange.size(); i++) {
				std::string TempCheckString = BoatRange[i];
				if (boatLabel == "boat5")
					BoatIndex = 0;
				if (boatLabel == "boat4")
					BoatIndex = 1;
				if (boatLabel == "boat3")
					BoatIndex = 2;
				if (boatLabel == "sub")
					BoatIndex = 3;
				if (boatLabel == "boat2")
					BoatIndex = 4;
				for (int j = 0; j < AllRanges.size(); j++) {
					if (AllRanges[j].size() > 1 && j != BoatIndex) {
						for (int k = 0; k < AllRanges[j].size(); k++) {
							if (TempCheckString == AllRanges[j][k])
								Collision = true;
						}
					}
				}

			}
			if (Collision == true) {
				this->object->moveRelative(Aftr::Vector(0, -1, 0));
				mat1 = this->object->getPose();
				x = mat1[12];
				y = mat1[13];
			}

		}
		ImGui::SameLine();
		//*****************************
		//	Right Button		(follows similar logic to forward)
		//*****************************
		if (ImGui::Button("Right", ImVec2(60, 60))) {
			bool OffMap = false;//var for detecting if the movement would be off of the map
			for (int i = 0; i < BoatRange.size(); i++) {//checks if any of the tiles the current boat is on is a right most tile if so it changes the offMap var
				if (BoatRange[i] == "M1" || BoatRange[i] == "M2" || BoatRange[i] == "M3" || BoatRange[i] == "M4" || BoatRange[i] == "M5" || BoatRange[i] == "M6" || BoatRange[i] == "M7" || BoatRange[i] == "M8" || BoatRange[i] == "M9" || BoatRange[i] == "M10" || BoatRange[i] == "M11" || BoatRange[i] == "M12" || BoatRange[i] == "M13")
					OffMap = true;
			}
			if (OffMap == false) {
				this->object->moveRelative(Aftr::Vector(0, -1, 0));
				mat1 = this->object->getPose();
				x = mat1[12];
				y = mat1[13];
			}
			updateBoatPos(BoatRange, AllRanges, MapBottom);
			int BoatIndex = 0;
			bool Collision = false;
			for (int i = 0; i < BoatRange.size(); i++) {
				std::string TempCheckString = BoatRange[i];
				if (boatLabel == "boat5")
					BoatIndex = 0;
				if (boatLabel == "boat4")
					BoatIndex = 1;
				if (boatLabel == "boat3")
					BoatIndex = 2;
				if (boatLabel == "sub")
					BoatIndex = 3;
				if (boatLabel == "boat2")
					BoatIndex = 4;
				for (int j = 0; j < AllRanges.size(); j++) {
					if (AllRanges[j].size() > 1 && j != BoatIndex) {
						for (int k = 0; k < AllRanges[j].size(); k++) {
							if (TempCheckString == AllRanges[j][k])
								Collision = true;
						}
					}
				}

			}
			if (Collision == true) {
				this->object->moveRelative(Aftr::Vector(0, 1, 0));
				mat1 = this->object->getPose();
				x = mat1[12];
				y = mat1[13];
			}

		}
		ImGui::Indent(30.0f);
		//*****************************
		//	Backwards Button		(follows similar logic to forward)
		//*****************************
		if (ImGui::Button("Backward", ImVec2(80, 60))) {
			bool OffMap = false;//var for detecting if the movement would be off of the map
			for (int i = 0; i < BoatRange.size(); i++) {//checks if any of the tiles the current boat is on is a botrom tile if so it changes the offMap var
				if (BoatRange[i] == "A13" || BoatRange[i] == "B13" || BoatRange[i] == "C13" || BoatRange[i] == "D13" || BoatRange[i] == "E13" || BoatRange[i] == "F13" || BoatRange[i] == "G13" || BoatRange[i] == "H13" || BoatRange[i] == "I13" || BoatRange[i] == "J13" || BoatRange[i] == "K13" || BoatRange[i] == "L13" || BoatRange[i] == "M13")
					OffMap = true;
			}
			if (OffMap == false) {
				this->object->moveRelative(Aftr::Vector(-1, 0, 0));
				mat1 = this->object->getPose();
				x = mat1[12];
				y = mat1[13];
			}
			updateBoatPos(BoatRange, AllRanges, MapBottom);
			int BoatIndex = 0;
			bool Collision = false;
			for (int i = 0; i < BoatRange.size(); i++) {
				std::string TempCheckString = BoatRange[i];
				if (boatLabel == "boat5")
					BoatIndex = 0;
				if (boatLabel == "boat4")
					BoatIndex = 1;
				if (boatLabel == "boat3")
					BoatIndex = 2;
				if (boatLabel == "sub")
					BoatIndex = 3;
				if (boatLabel == "boat2")
					BoatIndex = 4;
				for (int j = 0; j < AllRanges.size(); j++) {
					if (AllRanges[j].size() > 1 && j != BoatIndex) {
						for (int k = 0; k < AllRanges[j].size(); k++) {
							if (TempCheckString == AllRanges[j][k])
								Collision = true;
						}
					}
				}

			}
			if (Collision == true) {
				this->object->moveRelative(Aftr::Vector(1, 0, 0));
				mat1 = this->object->getPose();
				x = mat1[12];
				y = mat1[13];
			}

		}
		//*****************************
		//	Rotate Button
		//*****************************
		if (ImGui::Button("Rotate", ImVec2(80, 60))) {
			bool OffMap = false;//var for detecting if the movement would be off of the map
			std::string lbl = this->object->getLabel();
			if (boatLabel == "boat5" || boatLabel == "boat4") {
				for (int i = 0; i < BoatRange.size(); i++) {//if it is one of the longer boats we need to check the second most outer range of the tiles
					if (BoatRange[i] == "B12" || BoatRange[i] == "C12" || BoatRange[i] == "D12" || BoatRange[i] == "E12" || BoatRange[i] == "F12" || BoatRange[i] == "G12" || BoatRange[i] == "H12" || BoatRange[i] == "I12" || BoatRange[i] == "J12" || BoatRange[i] == "K12" || BoatRange[i] == "L12")
						OffMap = true;
					if (BoatRange[i] == "B2" || BoatRange[i] == "C2" || BoatRange[i] == "D2" || BoatRange[i] == "E2" || BoatRange[i] == "F2" || BoatRange[i] == "G2" || BoatRange[i] == "H2" || BoatRange[i] == "I2" || BoatRange[i] == "J2" || BoatRange[i] == "K2" || BoatRange[i] == "L2")
						OffMap = true;
					if (BoatRange[i] == "L2" || BoatRange[i] == "L3" || BoatRange[i] == "L4" || BoatRange[i] == "L5" || BoatRange[i] == "L6" || BoatRange[i] == "L7" || BoatRange[i] == "L8" || BoatRange[i] == "L9" || BoatRange[i] == "L10" || BoatRange[i] == "L11" || BoatRange[i] == "L12")
						OffMap = true;
					if (BoatRange[i] == "B2" || BoatRange[i] == "B3" || BoatRange[i] == "B4" || BoatRange[i] == "B5" || BoatRange[i] == "B6" || BoatRange[i] == "B7" || BoatRange[i] == "B8" || BoatRange[i] == "B9" || BoatRange[i] == "B10" || BoatRange[i] == "B11" || BoatRange[i] == "B12")
						OffMap = true;
				}

			}
			else {

			for (int i = 0; i < BoatRange.size(); i++) {//if it is one of the smaller boats we need to check the outer most tiles
				if (BoatRange[i] == "A13" || BoatRange[i] == "B13" || BoatRange[i] == "C13" || BoatRange[i] == "D13" || BoatRange[i] == "E13" || BoatRange[i] == "F13" || BoatRange[i] == "G13" || BoatRange[i] == "H13" || BoatRange[i] == "I13" || BoatRange[i] == "J13" || BoatRange[i] == "K13" || BoatRange[i] == "L13" || BoatRange[i] == "M13")
					OffMap = true;
				if (BoatRange[i] == "A1" || BoatRange[i] == "B1" || BoatRange[i] == "C1" || BoatRange[i] == "D1" || BoatRange[i] == "E1" || BoatRange[i] == "F1" || BoatRange[i] == "G1" || BoatRange[i] == "H1" || BoatRange[i] == "I1" || BoatRange[i] == "J1" || BoatRange[i] == "K1" || BoatRange[i] == "L1" || BoatRange[i] == "M1")
					OffMap = true;
				if (BoatRange[i] == "M1" || BoatRange[i] == "M2" || BoatRange[i] == "M3" || BoatRange[i] == "M4" || BoatRange[i] == "M5" || BoatRange[i] == "M6" || BoatRange[i] == "M7" || BoatRange[i] == "M8" || BoatRange[i] == "M9" || BoatRange[i] == "M10" || BoatRange[i] == "M11" || BoatRange[i] == "M12" || BoatRange[i] == "M13")
					OffMap = true;
				if (BoatRange[i] == "A1" || BoatRange[i] == "A2" || BoatRange[i] == "A3" || BoatRange[i] == "A4" || BoatRange[i] == "A5" || BoatRange[i] == "A6" || BoatRange[i] == "A7" || BoatRange[i] == "A8" || BoatRange[i] == "A9" || BoatRange[i] == "A10" || BoatRange[i] == "A11" || BoatRange[i] == "A12" || BoatRange[i] == "A13")
					OffMap = true;
			}

			}
			if (OffMap == false) {
				mat1 = this->object->getPose();
				x = mat1[12];
				y = mat1[13];

				if (lbl == "boat5" || lbl == "boat3" || lbl == "sub") {
					this->object->rotateAboutGlobalZ(-1.5708);

				}
				if (lbl == "boat2" && x + 0.5 < 35 && y - 0.5 > 9.2) {
					this->object->rotateAboutGlobalZ(-1.5708);
					this->object->moveRelative(Aftr::Vector(0.5, -0.5, 0));
				}
				if (lbl == "boat4" && x + 0.5 < 35 && y - 0.5 > 9.2) {
					this->object->rotateAboutGlobalZ(-1.5708);
					this->object->moveRelative(Aftr::Vector(0.5, -0.5, 0));
				}
			}
			updateBoatPos(BoatRange, AllRanges, MapBottom); // update tile ranges
			int BoatIndex = 0;
			bool Collision = false;//check for collisions 
			for (int i = 0; i < BoatRange.size(); i++) {
				std::string TempCheckString = BoatRange[i];
				if (boatLabel == "boat5")
					BoatIndex = 0;
				if (boatLabel == "boat4")
					BoatIndex = 1;
				if (boatLabel == "boat3")
					BoatIndex = 2;
				if (boatLabel == "sub")
					BoatIndex = 3;
				if (boatLabel == "boat2")
					BoatIndex = 4;
				for (int j = 0; j < AllRanges.size(); j++) {
					if (AllRanges[j].size() > 1 && j != BoatIndex) {
						for (int k = 0; k < AllRanges[j].size(); k++) {
							if (TempCheckString == AllRanges[j][k])
								Collision = true;
						}
					}
				}

			}
			if (Collision == true) {//if there was a collision undo the rotation
				if (lbl == "boat5" || lbl == "boat3" || lbl == "sub") {
					this->object->rotateAboutGlobalZ(1.5708);

				}
				if (lbl == "boat2" && x + 0.5 < 35 && y - 0.5 > 9.2) {
					this->object->rotateAboutGlobalZ(1.5708);
					this->object->moveRelative(Aftr::Vector(-0.5, 0.5, 0));
				}
				if (lbl == "boat4" && x + 0.5 < 35 && y - 0.5 > 9.2) {
					this->object->rotateAboutGlobalZ(1.5708);
					this->object->moveRelative(Aftr::Vector(-0.5, 0.5, 0));
				}
			}



		}
		//*************************************************
		//	Done Button			 (when setup is complete)
		//*************************************************
		if (ImGui::Button("Done", ImVec2(80, 60))) {
			//check if all boats are init
			bool AllPlaced = true;
			static bool msgSent = false;
			for (int i = 0; i < initPlacement.size(); i++) {
				if (initPlacement[i] == false)
					AllPlaced = false;
			}
			if (AllPlaced == false) {
				std::cout << "you still have boat/s to place";
			}
			else {
				if (msgSent == false) {
				msg.Positon = "SETUP";
				for (int i = 0; i < AllRanges.size(); i++) {
					for (int j = 0; j < AllRanges[i].size(); j++) {
						msg.Positon = msg.Positon + " " + AllRanges[i][j];
					}

				}
				this->client->sendNetMsgSynchronousTCP(msg);
				msgSent = true;
				Done_With_setup = true;
				static int DoneCheck2 = ManagerGLView::getGLView<GLViewBattleship>()->doneSetup;
				if (DoneCheck2 == 1) {
					ManagerGLView::getGLView<GLViewBattleship>()->myTurn = true;
				}
				}
				else {
					static int DoneCheck2 = ManagerGLView::getGLView<GLViewBattleship>()->doneSetup;
					std::cout << "DoneChec2k= " << DoneCheck2;
				}
			}

		}

		ImGui::End();
	}
}



//function for updating the positon of each boat 
//
//Input: BoatRange - range of tiles for currently selected boat
//		 AllRanges - range of tiles for all boats
//		 MapBottom - mapping of bottom board containing a string of tile name and x,y position coordinates
//
//
void GuiWOclass::updateBoatPos(std::vector<std::string>& BoatRange, std::vector<std::vector<std::string>>& AllRanges, std::vector<std::pair<std::string, std::pair<float, float> >>& MapBottom) {
	std::pair<float, float> tempPair;
	Aftr::Vector LookDir = this->object->getLookDirection();
	Mat4 mat = this->object->getPose();
	std::string boatLabel = this->object->getLabel();
	if (boatLabel == "boat2") {
		if (LookDir[0] == 1 || LookDir[0] == -1) {//if boat is vertical
			mat[12] = mat[12] + 0.5;
		}
		if (LookDir[1] == 1 || LookDir[1] == -1) {//if boat is horizontal
			mat[13] = mat[13] - 0.5;
		}
	}
	if (boatLabel == "boat4") {
		if (LookDir[0] == 1 || LookDir[0] == -1) {//if boat is vertical
			mat[13] = mat[13] - 0.5;
		}
		if (LookDir[1] == 1 || LookDir[1] == -1) {//if boat is horizontal
			mat[12] = mat[12] + 0.5;
		}
	}

	for (int i = 0; i < MapBottom.size(); i++) {
		tempPair = MapBottom[i].second;
		if (((tempPair.first + 0.1) > mat[13] && (tempPair.first - 0.1) < mat[13]) && ((tempPair.second + 0.1) > mat[12] && (tempPair.second - 0.1) < mat[12])) {
			if (LookDir[0] == 1 || LookDir[0] == -1) {//if boat is vertical
				if (boatLabel == "boat5") {//push back the tiles boat would be on top of
					BoatRange.push_back(MapBottom[i - 26].first);
					BoatRange.push_back(MapBottom[i - 13].first);
					BoatRange.push_back(MapBottom[i].first);
					BoatRange.push_back(MapBottom[i + 13].first);
					BoatRange.push_back(MapBottom[i + 26].first);
					AllRanges[0] = BoatRange;
				}
				if (boatLabel == "boat4") {//push back the tiles boat would be on top of
					BoatRange.push_back(MapBottom[i - 2].first);
					BoatRange.push_back(MapBottom[i - 1].first);
					BoatRange.push_back(MapBottom[i].first);
					BoatRange.push_back(MapBottom[i + 1].first);
					AllRanges[1] = BoatRange;
				}
				if (boatLabel == "boat3" || boatLabel == "sub") {//push back the tiles boat would be on top of
					BoatRange.push_back(MapBottom[i - 13].first);
					BoatRange.push_back(MapBottom[i].first);
					BoatRange.push_back(MapBottom[i + 13].first);
					if (boatLabel == "boat3")
						AllRanges[2] = BoatRange;
					if (boatLabel == "sub")
						AllRanges[3] = BoatRange;
				}
				if (boatLabel == "boat2") {//push back the tiles boat would be on top of
					BoatRange.push_back(MapBottom[i].first);
					BoatRange.push_back(MapBottom[i + 13].first);
					AllRanges[4] = BoatRange;
				}

			}
			if (LookDir[1] == 1 || LookDir[1] == -1) {//if boat is horizontal
				if (boatLabel == "boat5") {//push back the tiles boat would be on top of
					BoatRange.push_back(MapBottom[i - 2].first);
					BoatRange.push_back(MapBottom[i - 1].first);
					BoatRange.push_back(MapBottom[i].first);
					BoatRange.push_back(MapBottom[i + 1].first);
					BoatRange.push_back(MapBottom[i + 2].first);
					AllRanges[0] = BoatRange;
				}
				if (boatLabel == "boat4") {//push back the tiles boat would be on top of


					BoatRange.push_back(MapBottom[i - 13].first);
					BoatRange.push_back(MapBottom[i].first);
					BoatRange.push_back(MapBottom[i + 13].first);
					BoatRange.push_back(MapBottom[i + 26].first);
					AllRanges[1] = BoatRange;
				}
				if (boatLabel == "boat3" || boatLabel == "sub") {//push back the tiles boat would be on top of
					BoatRange.push_back(MapBottom[i - 1].first);
					BoatRange.push_back(MapBottom[i].first);
					BoatRange.push_back(MapBottom[i + 1].first);
					if (boatLabel == "boat3")
						AllRanges[2] = BoatRange;
					if (boatLabel == "sub")
						AllRanges[3] = BoatRange;
				}
				if (boatLabel == "boat2") {//push back the tiles boat would be on top of
					BoatRange.push_back(MapBottom[i - 1].first);
					BoatRange.push_back(MapBottom[i].first);
					AllRanges[4] = BoatRange;
				}
			}

		}

	}

}

//function for handeling the guess made and sending a net msg to the other client about it
//input: guess - a string containing the tile of the guess that was selected
//		 Map - Mapping of the top board containing a string of the tile and a y,z coord pair
//
void GuiWOclass::handleGuess(std::string guess, std::vector<std::pair<std::string, std::pair<float, float> >> Map) {

	Aftr::WorldContainer* World = ManagerGLView::getGLView()->getWorldContainer();//get worldLst
	std::string miss(ManagerEnvironmentConfiguration::getSMM() + "../../modules/Battleship/mm/models/miss.stl");//model for misses
	std::string hit(ManagerEnvironmentConfiguration::getSMM() + "../../modules/Battleship/mm/models/hit.stl");//model for hits
	bool missShot = true;//for determining if guess was a hit or miss
	std::stringstream ssBoatPos(ManagerGLView::getGLView<GLViewBattleship>()->EnemyBoatPos);//convert the opponets boats to a string stream

	std::string tmpPosition;
	for (int i = 0; i < 17; i++) {
		ssBoatPos >> tmpPosition;
		if (tmpPosition == guess) {
			missShot = false;
		}
		tmpPosition = "";
	}

	std::pair<float, float> tmpPair;
	for (int i = 0; i < Map.size(); i++) {
		if (Map[i].first == guess) {
			tmpPair = Map[i].second;
		}

	}

	if (missShot == true) {//if it was a miss generate world object on the top with the miss model
		WO* wo = WO::New(miss, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
		wo->setPosition(Vector(36.5, tmpPair.first, tmpPair.second));//for top
		wo->rotateAboutGlobalZ(-1.5708);
		wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
		wo->setLabel("missguess");
		World->push_back(wo);
	}
	else if (missShot == false) {//if it was a hit generate world object on the top with the hit model
		WO* wo = WO::New(hit, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
		wo->setPosition(Vector(36.5, tmpPair.first, tmpPair.second));//for top
		wo->rotateAboutGlobalZ(-1.5708);
		wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
		wo->setLabel("hitguess");
		World->push_back(wo);
	
	
	}

	msg.Positon = "SHOT";//send string SHOT that will be checked in the net msg
	msg.Positon = msg.Positon + " " + guess;//inclued the guess
	if (missShot == true) {//finally add if it was a hit or miss
		msg.Positon = msg.Positon + " " + "MISS";
	}
	if (missShot == false) {
		msg.Positon = msg.Positon + " " + "HIT";
	}
	this->client->sendNetMsgSynchronousTCP(msg);//send msg to other client
	ManagerGLView::getGLView<GLViewBattleship>()->myTurn = false;//end turn


}

// sets the curretn WO being modified
void GuiWOclass::setCurrentWO(Aftr::WO* obj) {
	this->object = obj;

}

//sets the ship piecesto a vector of WO
void GuiWOclass::setShips(std::vector<WO*> Ships) {
	this->Ship_pieces = Ships;


}

#endif