#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "zerogui/ZeroGUI.h"
#include <map>
#include <TlHelp32.h>
#include "SimpleIni.h"
#include <bitset>
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#include "TFD_SDK.h"


#define IsKeyPressed(key) GetAsyncKeyState(key) & 1 && GetAsyncKeyState(key) & 0x8000
#define IsKeyHeld(key) (GetAsyncKeyState(key) & 0x8000)

struct CONTROLLER_STATE
{
	XINPUT_STATE state;
	bool bConnected;
};

CONTROLLER_STATE g_Controllers[4];
HRESULT UpdateControllerState();

/*
*  Hooks
*/
static __int64 (*M1org)(void* ViewportClient, void* DebugCanvas) = NULL;
static __int64 YourHookProc(void* ViewportClient, void* DebugCanvas);
//static __int64 YourHookProc_old(void* self, void* Canvas);
ULONG64* GetDrawTransitionVTableAddress(TFD_SDK::UGameViewportClient* vp);
DWORD WINAPI Init(HMODULE Module);

/*
*  Signatures
*/
const char* GNamesSig = "\x4C\x8D\x35\x00\x00\x00\x00\x0F\x95\x44\x24\x2C\x48\x2B\xCB";
const char* GNamesMask = "xxx????xxxxxxx";
//const char* GObjectsSig = "\x4C\x8B\x0D\x00\x00\x00\x00\x8B\xD0";
//const char* GObjectsMask = "xxx????xx";
const char* GObjectsSig = "\x80\x08\x00\x37\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x21";
const char* GObjectsMask = "xx?xxxxxxxxxxxxxxxx";
// 80 08 DF 37 00 00 00 00 00 00 00 00 00 00 00 00 00 00 21
//const char* NoSpreadSig =	"\xF3\x0F\x5C\x00\xF3\x0F\x5F\xF2\xF3\x0F\x5D\xF7\x0F\x28\xC6";
//const char* NoSpreadMask =	"xxx?xxxxxxxxxxx";
//const char* NoSpreadSig = "\xF3\x41\x0F\x5D\xF6\x44\x0F\x28\xC6";
//const char* NoSpreadMask = "xxxxxxxxx";
// // F3 45 0F 5C C9 90 90 90 90
// 74 7D E8 ?? ?? ?? ?? 48 8B 53 10 <- add 0x16 to signature address to get to the JE, change JE to JNE
// old pattern is 48 8B 86 D0 00 00 00 F3 44 0F 11 86 - 0x9
//const char* NoSpreadSig = "\xE8\x00\x00\x00\x00\x48\x8B\x86\xD0\x00\x00\x00\x48\x8B\x98";
//const char* NoSpreadMask = "x????xxxxxxxxxx";
const char* NoSpreadSig = "\x48\x8B\x86\xD0\x00\x00\x00\xF3\x0F\x11\xBE\x08";
const char* NoSpreadMask = "xxxxxxxxxxxx";
uint8_t NoSpreadOriginal[9] = { 0xF3, 0x41, 0x0F, 0x5D, 0xF6, 0x44, 0x0F, 0x28, 0xC6 };
uint8_t NoSpreadCheat[9] = { 0xF3, 0x45, 0x0F, 0x5C, 0xC9, 0x90, 0x90, 0x90, 0x90};
uint8_t* NoSpreadAddress = 0;
//  Original 84 C0 74 1F 48 8B BE E8 00 00 00
const char* NoRecoilSig = "\x84\xC0\x74\x1F\x48\x8B\xBE\xF0\x00\x00\x00";
const char* NoRecoilMask = "xxxxxxxxxxx";
uint8_t* NoRecoilAddress = 0;
const char* RapidFireSig = "\x72\x00\xF3\x0F\x10\x87\x00\x00\x00\x00\x48";
const char* RapidFireMask = "x?xxxx????x";
uint8_t* RapidFireAddress = 0;



struct sigmod {
	uintptr_t dwBase, dwSize;
};
bool MemoryCompare(const BYTE* bData, const BYTE* bSig, const char* szMask);
uintptr_t FindSignature(int procID, sigmod mod, const char* sig, const char* mask);

/*
*  Data
*/
std::vector<int> Worlds;

bool CheckPointers();
TFD_SDK::UWorld* GWorld;
TFD_SDK::UWorld* OriginalWorld;
TFD_SDK::UEngine* GEngine;
TFD_SDK::ULocalPlayer* LocalPlayer;
TFD_SDK::AM1PlayerController* PlayerController;
bool inGame = false;
TFD_SDK::AM1PlayerControllerInGame* PlayerIngameController;
TFD_SDK::AM1Player* LocalCharacter;
TFD_SDK::UGameViewportClient* LocalView;
TFD_SDK::UM1ActorManagerSubsystem* Actors;
TFD_SDK::APlayerState* PlayerState;
bool isGUIInit = false;
void LoadCFG();
void SaveCFG();
CSimpleIniA ini;
float lastScreenSize = 0;
bool updateMiddle = true;
TFD_SDK::FVector2D ScreenMiddle = { 0, 0 };

/*
*  Overlay
*/
void DrawMenu();
TFD_SDK::FVector2D cfg_WindowPos = { 100, 100 };
bool cfg_DrawMenu = false;

const TFD_SDK::FLinearColor ColorWhite = { 1.0f, 1.0f, 1.0f, 0.95f };
const TFD_SDK::FLinearColor ColorNameRed = { 1.0f, 0, 0, 1.0f };
const TFD_SDK::FLinearColor ColorRed = { 0.8f, 0, 0, 0.8f };
const TFD_SDK::FLinearColor ColorBlue = { 0, 0, 1.0f, 0.50f };
const TFD_SDK::FLinearColor ColorDarkRed = { 0.6f, 0.0f, 0.0f, 0.6f };
const TFD_SDK::FLinearColor ColorGreen = { 0, 1.0f, 0, 1.0f };
const TFD_SDK::FLinearColor ColorGrey = { 0.8f, 0.8f, 0.8f, 0.8f };
const TFD_SDK::FLinearColor ColorMana = { 0, 1.0f, 1.0f, 1.0f };
const TFD_SDK::FLinearColor ColorPink = { 1.0f, 0, 1.0f, 1.0f };
const TFD_SDK::FLinearColor ColorSadPink = { 0.6f, 0.32f, 0.6f, 0.8f };

const TFD_SDK::FLinearColor ColorCommon = { 0, 1.0f, 0.451f, 0.5f };
const TFD_SDK::FLinearColor ColorUncommon = { 0.05f, 0.2f, 1.0f, 0.799f };
const TFD_SDK::FLinearColor ColorRare = { 1.0f, 0, 1.0f, 0.8f };
const TFD_SDK::FLinearColor ColorUltimate = { 1.0f, 0, 0, 1.0f };

const TFD_SDK::FLinearColor ColorGold = { 0.95f, 0.95f, 0.05f, 0.95f };
struct ColorEntry {
	const char* Name;
	TFD_SDK::FLinearColor Value;
};

bool WorldToScreen(const TFD_SDK::FVector& worldLoc, TFD_SDK::FVector2D* screenPos);

int BoneOffset = 0x508;
uintptr_t BonePtr = 0x0;
TFD_SDK::TArray<TFD_SDK::FTransform>* BoneArrayPointer = nullptr;
/*
*   Player Enemy ESP
*/
void PlayerEnemyESP();
bool cfg_EnablePlayerEnemyESP = false;
bool cfg_DrawPlayerNames = false;
bool cfg_DrawPlayerBoxes = false;
bool cfg_DrawPlayerLines = false;
bool cfg_DrawEnemyNames = false;
bool cfg_DrawEnemyBoxes = false;


/*
*  Item ESP and Vacuum
*/
void ItemESPVacuum();
bool cfg_EnableItemESP = false;
bool cfg_DrawItemBoxes = false;
bool cfg_DrawItemNames = false;
bool cfg_DrawItemLines = false;
bool cfg_DrawVaults = false;
bool cfg_DrawLootBoxes = false;
bool cfg_DrawLootBarrels = false;
bool cfg_LootVacuum = false;
float cfg_LootVacuumRange = 250000.0f;
int cfg_LootVacuumKey = 0x54;

void InstantInfiltration();
void EverythingESP();

/*
*  Aimbot
*/
void Aimbot();
TFD_SDK::AActor* GetClosestEnemy(int& ID);
bool cfg_EnableAimbot = false;
bool cfg_EnableAimbotHold = true;
bool cfg_EnableAimbotToggle = false;
int cfg_AimbotHoldKey = 0x02;
int cfg_AimbotToggleKey = 0x59;
float cfg_AimbotFOV = 150.0f;
float cfg_AimbotSmoothing = 20.0f;
TFD_SDK::AActor* Aimbot_Target;
int Aimbot_BoneIndex = -1;
bool cfg_AimbotNoSpread = false;
bool cfg_AimbotController = false;
bool cfg_AimbotNoRecoil = false;
bool cfg_AimbotRapidFire = false;
uint8_t Recoil[2] = { 0x74, 0x75 };
uint8_t RapidFire[2] = { 0x72, 0x77 };
int cfg_InstantInfilKey = VK_HOME;

//Customization By Tivmo//

bool cfg_DrawCirclesInstead = false;
bool cfg_DrawEnemyLines = false;
float cfg_DistanceScale = 2400.0f;
float cfg_ScaleFactor = 1.0f;
TFD_SDK::FVector2D cfg_ESPBox = { 20, 30 };

/*
*  Extras
*/
bool cfg_NoReload = false;
bool cfg_CacheEnemyNames = false;
bool cfg_CacheEnemyBones = false;
void InstantReload();
bool cfg_HotSwapOverlay = false;
int HotSwapIndex = 0;
std::vector<int> HotSwapCharacters = { 0, 0, 0, 0 };
int cfg_HotSwapKey = VK_TAB;
float cfg_TimeScale = 1.25f;
int cfg_TimeScaleKey = VK_F2;
int cfg_TimeScaleHoldKey = VK_CONTROL;

void WriteEnemyNamesData();
std::unordered_map<int, std::string> ReadEnemyNamesData();
void WriteEnemyBonesData();
std::unordered_map<int, std::vector<int>> ReadEnemyBonesData();

std::unordered_map<int, std::vector<int>> IDBoneMap = { };
bool BonesChanged = false;

std::unordered_map<int, std::string> IDNameMap =
{
{ 510500032, "Cryo Soldier" },
{ 510200103, "Fortress Guard" },
{ 510100020, "Storm Warrior" },
{ 510500024, "Mass Soldier" },
{ 530100026, "Elite Tracker" },
{ 510500005, "Follower Assault Soldier" },
{ 510100019, "Farseer" },
{ 510500003, "Vulgus Follower" },
{ 510500002, "The Soulless" },
{ 510100023, "Destroyer" },
{ 510500031, "Bomber" },
{ 510100025, "Warden" },
{ 510500009, "Follower Panzer" },
{ 510500020, "Storm Warrior" },
{ 510100048, "Spiker" },
{ 510500004, "Fire Spitter" },
{ 510100031, "Bomber" },
{ 510500007, "Follower Grenadier" },
{ 510100005, "Follower Assault Soldier" },
{ 510100030, "Sentinel" },
{ 510100003, "Vulgus Follower" },
{ 510121001, "Interrupter Slaughterer" },
{ 510100002, "The Soulless" },
{ 510100001, "Vulgus War-Slave" },
{ 510100008, "Follower Shield Bearer" },
{ 510100007, "Follower Grenadier" },
{ 510100006, "Follower Sniper" },
{ 510100024, "Mass Soldier" },
{ 530100007, "Elite Vulgus Grenadier" },
{ 510100004, "Fire Spitter" },
{ 520130003, "Greg the Star-Crossed" },
{ 520120022, "Split Cell Dmigor" },
{ 510100034, "Slayer" },
{ 510121005, "Punisher" },
{ 510100026, "Tracker" },
{ 510100016, "Executor" },
{ 510100029, "Ripper" },
{ 510100043, "Wasp" },
{ 510100021, "Reaper" },
{ 530100018, "Elite Slaughterer" },
{ 510500043, "Wasp" },
{ 510100036, "Evangelist" },
{ 510500016, "Executor" },
{ 510100037, "Piercer" },
{ 510100039, "Fortress Guard" },
{ 510500033, "Creature" },
{ 510100017, "Third Eye" },
{ 510100041, "Vanguard" },
{ 510100018, "Slaughterer" },
{ 510100015, "Marauder" },
{ 510100022, "Trapball" },
{ 520130012, "Eterllick the Tracker" },
{ 510500012, "Gazer" },
{ 510100011, "Blazer" },
{ 510100042, "Bug Spreader" },
{ 510121004, "Cutlasser" },
{ 510100010, "Devotee" },
{ 510500040, "Raider" },
{ 510100028, "Acolyte" },
{ 530100027, "Elite Predator" },
{ 510500010, "Devotee" },
{ 510100027, "Predator" },
{ 510500011, "Blazer" },
{ 510100038, "Abomination" },
{ 510100012, "Gazer" },
{ 510100009, "Follower Panzer" },
{ 530100038, "Elite Abomination" },
{ 510100047, "Bulgebrute" },
{ 510100033, "Creature" },
{ 510100013, "Zealot" },
{ 510100014, "Annihilator" },
{ 530100025, "Elite Warden" },
{ 520130005, "Narzas the Consuming Flame" },
{ 510200064, "Warden" },
{ 510100032, "Cryo Soldier" },
{ 530100020, "Elite Storm Warrior" },
{ 510121003, "Ember Maw" },
{ 520120003, "Genetically Enhanced Greg Clone" },
{ 510500030, "Sentinel" },
{ 510100040, "Raider" },
{ 510121000, "Interrupter Storm Warrior" },
{ 520130013, "Seudo the Dreamer" },
{ 510110001, "Seudo's Assault Turret" },
{ 510110002, "Seudo's Medical Turret" },
{ 510500035, "Tintail" },
{ 510100050, "Machine Mother" },
{ 510110003, "Marauder" },
{ 510100035, "Tintail" },
{ 530100041, "Elite Vanguard" },
{ 510100051, "Miscreant" },
{ 530100034, "Elite Slayer" },
{ 510500015, "Marauder" },
{ 510500049, "Illusionist" },
{ 530100019, "Elite Farseer" },
{ 520130009, "Gruncah the Undying" },
{ 510000302, "KingFisher" }
};
bool NamesChanged = false;
