#pragma once
#include "AftrConfig.h"

#ifdef AFTR_CONFIG_USE_IMGUI

#include "WOImGuiAbstract.h"
#include "GLViewBattleShip.h"
#include <string>
#include <vector>
#include <utility>
#include "AftrManagers.h"
#include "WorldContainer.h"
#include "NetMsgCreateObject.h"
#include "NetMessengerClient.h"
#include <sstream>

namespace Aftr
{

	class GuiWOclass : public WOImGuiAbstract
	{
	public:
		static GuiWOclass* New(WOGUI* parentWOGUI, float width = 1.0f, float height = 1.0f);
		virtual ~GuiWOclass();

		///The main rendering method for the Dear ImGui. A user overrides this and places
		///all ImGui:: calls here beginning with ImGui::Begin() and ending with ImGui::End().
		virtual void drawImGui_for_this_frame();
		virtual void onUpdateWO() override;
		virtual void handleGuess(std::string guess, std::vector<std::pair<std::string, std::pair<float, float> >> Map);
		virtual void setCurrentWO(Aftr::WO* obj);
		virtual void setShips(std::vector<WO*> Ships);
		virtual void updateBoatPos(std::vector<std::string> &BoatRange, std::vector<std::vector<std::string>> &AllRanges, std::vector<std::pair<std::string, std::pair<float, float> >> &MapBottom);
		//virtual void MSGRECIVED();
		//virtual void setCurrentWorld();
		///Just an informal example on how to draw an Aftr::Texture as a image in ImGui.
		///This is quite fast because ImGui uses the same underlying GLint texture that
		///Aftr::Texture wraps.
		//void drawImGui_Texture(Texture* tex);
		WO* object;
		std::vector<WO*> Ship_pieces;

		NetMessengerClient* client;
		NetMsgCreateObject msg;
	protected:
		virtual void onCreate(float width, float height) override;
		GuiWOclass(WOGUI* parentWOGUI);

		//std::filesystem::path selected_path{ "./" };
		//std::string origin_fileDialog = "None"; //not essential at all -- simply shows how title is used as a hashed key for file dialog

	};

}



#endif