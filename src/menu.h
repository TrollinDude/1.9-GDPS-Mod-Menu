#pragma once

bool menu = false;
bool menuOn = false;
bool tab = false;


DWORD base = (DWORD)GetModuleHandleA(0);
DWORD libcocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");
DWORD mhMenu = (DWORD)GetModuleHandleA("extensions.dll");


struct
{

	//
	//	funtions
	//


	//gd func
	int getIsVerified				= base + 0x548E0;
	int playLevelLayer_init			= base + 0x9BC10;
	int menuLayer_init				= base + 0xD5F50;
	int PlayLayer_init				= base + 0xE35D0;
	int playerPhysicsManager		= base + 0xE9360;
	int PlayLayer_destoryPlayer		= base + 0xF04A0;
	int mirrorPortalManager			= base + 0xF0BE0;


	//cocos func
	int initParticleSystem			= libcocosbase + 0xb4730;


	//
	//	strings
	//
	int attemptText					= base + 0x146B64; // Attempt %i


} addy;



std::string filePath;
std::ifstream inputFile;
nlohmann::json jsonData;


struct {

	bool noclip = false;
	bool editLevel = false;
	bool verifiyHack = false;
	bool noMirror = false;
	bool noDeathEffect = false;
	bool freezePlayer = false;
	bool noParticles = false;
	bool hideAttempts = false;
	bool instantMirror = false;
	bool copyHack = false;
	bool unlockMainLevels = false;
	bool textLength = false;
	bool smoothEditorTrail = false;
	bool noRespawnBlink = false;
	bool noWavePulse = false;
	bool practicePulse = false;
	bool noTransition = false;
	bool sliderLimitBypass = false;
	bool noTrail = false;
	bool textFilterBypass = false;
	bool freeWindowResize = false;
	bool instantComplete = false;

}settings;


bool onstart1 = true;
bool onstart2 = true;
bool onstart3 = true;
bool onstart4 = true;
bool onstart5 = true;

char dllFilePath[1024];

//used for the variable hack input box and varible viewer
std::string varType = "Int";
std::vector<uintptr_t> offs = {};
int ptrBase = 0x0;
float valF = 0.0f;
int valI = 0;
char varBuffer[2048];
std::string varString = "0";


void LoadSettings() {
	auto file = fopen(".gd\\mod_save.dat", "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		auto size = ftell(file);

		if (size == sizeof(settings)) {
			fseek(file, 0, SEEK_SET);
			fread(&settings, sizeof(settings), 1, file);
			fclose(file);
		}
	}
}


void SaveSettings() {
	auto file = fopen(".gd\\mod_save.dat", "wb");
	if (file) {
		fwrite(&settings, sizeof(settings), 1, file);
		fclose(file);
	}
}



//Utils for the variable hack input box and varible viewer and also the patching system

template <typename T>
std::vector<T> parseBytes(std::string byteString) {
	std::vector<T> bytes;
	std::string byte;
	for (int i = 0; i < byteString.length(); i++) {
		if (byteString[i] == ' ') {
			bytes.push_back((T)std::stoi(byte, nullptr, 16));
			byte = "";
		}
		else {
			byte += byteString[i];
		}
	}
	bytes.push_back((T)std::stoi(byte, nullptr, 16));
	return bytes;
}

int hexStringToInt(std::string hex) {
	int value;
	std::stringstream ss;
	ss << std::hex << hex;
	ss >> value;
	return value;
}

void patchMod(const char* name, bool* toggleVar) {

	auto mod = jsonData["Patches"][std::string(name)];

	ImGui::Checkbox(name, toggleVar);
	if (*toggleVar) {
		for (auto const& i : mod["Patches"]) {

			if (i["Module"] == "base") {
				amemory::write_bytes_protect(base + hexStringToInt(i["Addr"]), parseBytes<uint8_t>(i["On"]));
			}
			else if (i["Module"] == "libcocos") {
				amemory::write_bytes_protect(libcocosbase + hexStringToInt(i["Addr"]), parseBytes<uint8_t>(i["On"]));
			}
		}
	}
	else {
		for (auto const& i : mod["Patches"]) {

			if (i["Module"] == "base") {
				amemory::write_bytes_protect(base + hexStringToInt(i["Addr"]), parseBytes<uint8_t>(i["Off"]));
			}
			else if (i["Module"] == "libcocos") {
				amemory::write_bytes_protect(libcocosbase + hexStringToInt(i["Addr"]), parseBytes<uint8_t>(i["Off"]));
			}

		}
	}
}
