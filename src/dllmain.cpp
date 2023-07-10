#define _CRT_SECURE_NO_WARNINGS

#include "includes.h"
#include <cocos2d.h>

using namespace cocos2d;

//parse string to array of bytes: example
// "8A 81 C5 01 00 00" to { 0x8A, 0x81, 0xC5, 0x01, 0x00, 0x00 }

//allow for the user to call with a template such as uint8_t, uintptr etc

void RenderMain() {

	const auto show = true;
	const auto director = CCDirector::sharedDirector();
	const bool enable_touch = !ImGui::GetIO().WantCaptureMouse || !show;
	director->getTouchDispatcher()->setDispatchEvents(enable_touch);

	if (GetAsyncKeyState(VK_TAB) < 0 && tab == false)
	{
		menu = !menu;
		tab = true;
	}
	if (GetAsyncKeyState(VK_TAB) == 0 && tab == true)
	{
		tab = false;
	}


	int enable = amemory::load_pointer(mhMenu + 0x75904, { 0x144 }); // isMhModMenuEnabled
	int enablea = amemory::read<int>(enable);

	if (enable == 0) { // if fps bypass is enabled
		menuOn = menu;
	}
	else {
		menuOn = enablea;
	}



	if (menuOn)
	{

		ImGui::Begin("Bypass");

		if (onstart1)
		{
			ImGui::SetWindowSize(ImVec2(210, 360));
			ImGui::SetWindowPos(ImVec2(10, 150));
			onstart1 = false;

		}

		patchMod("Free Window Resize", &settings.freeWindowResize);

		patchMod("Slider Limit Bypass", &settings.sliderLimitBypass);

		patchMod("Text Filter Bypass", &settings.textFilterBypass);

		patchMod("Text Length Bypass", &settings.textLength);

		patchMod("Unlock Main Levels", &settings.unlockMainLevels);

		patchMod("Verify Bypass", &settings.verifiyHack);
		

		ImGui::End();



		ImGui::Begin("Gameplay");

		if (onstart2)
		{
			ImGui::SetWindowSize(ImVec2(210, 480));
			ImGui::SetWindowPos(ImVec2(230, 150));
			onstart2 = false;
		}

		patchMod("Freeze Player", &settings.freezePlayer);

		ImGui::Checkbox("Instant Complete", &settings.instantComplete);
		if (settings.instantComplete) {
			int addy = amemory::load_pointer(base + 0x16C1C4, { 0x144, 0x2A4, 0x4A8 });
			amemory::write_protect<float>(addy, 3.402823E+38);
		}

		patchMod("Instant Mirror", &settings.instantMirror);

		patchMod("Hide Attempts", &settings.hideAttempts);

		patchMod("Noclip", &settings.noclip);

		patchMod("No Death Effect", &settings.noDeathEffect);

		patchMod("No Mirror", &settings.noMirror);

		patchMod("No Particles", &settings.noParticles);

		patchMod("No Respawn Blink", &settings.noRespawnBlink);

		patchMod("No Trail", &settings.noTrail);

		patchMod("No Transition", &settings.noTransition);

		patchMod("No Wave Pulse", &settings.noWavePulse);

		if (settings.noWavePulse) {
			amemory::push_write(base + 0xD9E25, base + 0x14A39C);
		}
		else {
			amemory::write_bytes_protect(base + 0xD9E25, { 0xC8, 0x04, 0x00 });
		}

		ImGui::Begin("Creator");

		if (onstart1) {
			ImGui::SetWindowSize(ImVec2(210, 360));
			ImGui::SetWindowPos(ImVec2(450, 150));
			onstart1 = false;
		}

		patchMod("Copy Hack", &settings.copyHack);

		patchMod("Edit Level", &settings.editLevel);

		patchMod("Smooth Editor Trail", &settings.smoothEditorTrail);




		ImGui::Begin("Variable");


		if (onstart4) {
			ImGui::SetWindowSize(ImVec2(210, 110));
			ImGui::SetWindowPos(ImVec2(10, 520));

			onstart4 = false;
		}

		const char* items[] = {
			"Player Xpos",
			"Player Ypos",
			"Player Size",
			"Player Speed",
			"Normal Percent",
			"Practice Percent",
			"Session Attempts",
			"Level Attempts",
			"Level Jumps",
			"Block X",
			"Block Y",
			"Block ID"
		};

		static const char* currentItem = "Player Xpos";

		initVariableSystem(currentItem, items);


		ImGui::End();



		ImGui::Begin("Bot");

		ImGui::Text("bot/macro coming soon");

		ImGui::End();

	}

}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

		base = (DWORD)GetModuleHandleA(0); // DontRenameMeThxDash.exe
		libcocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");
		mhMenu = (DWORD)GetModuleHandleA("extensions.dll");

		LoadSettings();

		char currentDirectory[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, currentDirectory);

		filePath = currentDirectory;
		filePath += "\\.gd\\mods.json";

		inputFile.open(filePath);
		inputFile >> jsonData;

		::DisableThreadLibraryCalls(hModule);
        ::ImGuiHook::Load(RenderMain);

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		SaveSettings();
		FreeConsole();
        break;
    }
    return TRUE;
}

