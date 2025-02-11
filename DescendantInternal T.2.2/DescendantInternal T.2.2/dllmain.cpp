
#include "includes.h"
#include <thread>
#include <fstream>

// Uncomment the line below for console window + some debug prints
//#define IS_DEBUG



bool CheckPointers()
{
	if (!GEngine)
		GEngine = TFD_SDK::UEngine::GetEngine();
	if (GEngine)
	{
		GWorld = nullptr;
		for (int i = 0; i < TFD_SDK::UObject::GObjects->Num(); i++)
		{
			TFD_SDK::UObject* Obj = TFD_SDK::UObject::GObjects->GetByIndex(i);

			if (!Obj)
				continue;

			if (Obj->Flags & TFD_SDK::EObjectFlags::BeginDestroyed ||
				Obj->Flags & TFD_SDK::EObjectFlags::BeingRegenerated ||
				Obj->Flags & TFD_SDK::EObjectFlags::FinishDestroyed ||
				Obj->Flags & TFD_SDK::EObjectFlags::NeedInitialization ||
				Obj->Flags & TFD_SDK::EObjectFlags::WillBeLoaded)
				continue;

			if (Obj->Flags & TFD_SDK::EObjectFlags::LoadCompleted && Obj->IsA(TFD_SDK::UWorld::StaticClass()) && !Obj->IsDefaultObject())
			{
				TFD_SDK::UWorld* World = static_cast<TFD_SDK::UWorld*>(Obj);
				if (World->OwningGameInstance)
				{
					if (World->OwningGameInstance->IsA(TFD_SDK::UM1GameInstance::StaticClass()))
					{
						std::string Name = World->Name.ToString();
						if (Name != "" && Name != "None")
						{
							GWorld = World;
							break;
						}
					}
				}
			}
		}
		if (GWorld && isGUIInit)
		{
			if (GWorld->IsA(TFD_SDK::UWorld::StaticClass()) && !GWorld->IsDefaultObject())
			{
				if (GWorld->OwningGameInstance && GWorld->OwningGameInstance->IsA(TFD_SDK::UM1GameInstance::StaticClass()))
				{
					std::string Name = GWorld->Name.ToString();
					if (Name != "" && Name != "None")
					{
						if (Name != "Lobby_P" && Name != "Level_Transition")
						{
							if (GWorld->OwningGameInstance->LocalPlayers && GWorld->OwningGameInstance->LocalPlayers[0] && GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController)
							{

								if (GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->IsA(TFD_SDK::AM1PlayerController::StaticClass()))
								{
									TFD_SDK::AM1PlayerController* PC = static_cast<TFD_SDK::AM1PlayerController*>(GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController);
									if (PC->Character && PC->Character->IsA(TFD_SDK::AM1Player::StaticClass()) && PC->ActorManager_Subsystem && PC->ActorManager_Subsystem->IsA(TFD_SDK::UM1ActorManagerSubsystem::StaticClass()))
									{
										PlayerController = PC;
										PlayerState = PC->PlayerState;
										LocalPlayer = GWorld->OwningGameInstance->LocalPlayers[0];
										LocalCharacter = static_cast<TFD_SDK::AM1Player*>(PlayerController->Character);
										Actors = PC->ActorManager_Subsystem;
										ZeroGUI::controller = GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
										if (PlayerController->IsA(TFD_SDK::AM1PlayerControllerInGame::StaticClass()))
										{
											PlayerIngameController = static_cast<TFD_SDK::AM1PlayerControllerInGame*>(PlayerController);
											inGame = true;
										}
										else
										{
											inGame = false;
											PlayerIngameController = nullptr;
										}

										return true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	PlayerController = nullptr;
	PlayerState = nullptr;
	LocalPlayer = nullptr;
	LocalCharacter = nullptr;
	Actors = nullptr;
	PlayerIngameController = nullptr;
	inGame = false;
	return false;
}

bool WorldToScreen(const TFD_SDK::FVector& worldLoc, TFD_SDK::FVector2D* screenPos)
{
	// This might be needed agan?
	//bool isOnScreen = LocalPlayer->PlayerController->ProjectWorldLocationToScreen(worldLoc, screenPos, true);
	//if (isOnScreen && ZeroGUI::canvas->SizeX > 1920)
	//{
	//	screenPos->X *= (1920.0f / ZeroGUI::canvas->SizeX);
	//	screenPos->Y *= (1080.0f / ZeroGUI::canvas->SizeY);
	//}
	bool isOnScreen = TFD_SDK::UGameplayStatics::ProjectWorldToScreen(PlayerController, worldLoc, screenPos, true);
	return isOnScreen;
}


//static __int64 YourHookProc_old(void* self, void* Canvas)
//{
//	if (Canvas)
//	{
//		if (IsKeyPressed(VK_INSERT))
//			CheckPointers();
//	}
//
//	return M1org(self, Canvas);
//}

static __int64 YourHookProc(void* self, void* Canvas)
{
	if (Canvas)
	{
		if (!isGUIInit)
		{
			ZeroMemory(g_Controllers, sizeof(CONTROLLER_STATE) * 4);
			CheckPointers();
			TFD_SDK::UCanvas* myCanvas = static_cast<TFD_SDK::UCanvas*>(Canvas);
			if (myCanvas->SizeX != lastScreenSize)
			{
				updateMiddle = true;
				lastScreenSize = myCanvas->SizeX;
			}
			ZeroGUI::CurrentFont = GEngine->SmallFont;
			ZeroGUI::canvas = myCanvas;
			ZeroGUI::engine = GEngine;

			if (cfg_CacheEnemyNames)
			{
				IDNameMap = ReadEnemyNamesData();
			}

			if (cfg_CacheEnemyBones)
			{
				IDBoneMap = ReadEnemyBonesData();
			}

			if (cfg_AimbotNoSpread)
			{
				DWORD old;
				VirtualProtect(NoSpreadAddress, sizeof(uint8_t) * 9, PAGE_EXECUTE_READWRITE, &old);
				memcpy(NoSpreadAddress, &NoSpreadCheat, sizeof(uint8_t) * 9);
				VirtualProtect(NoSpreadAddress, sizeof(uint8_t) * 9, old, NULL);
			}

			if (cfg_AimbotNoRecoil)
			{
				DWORD old;
				VirtualProtect(NoRecoilAddress, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
				memcpy(NoRecoilAddress, &Recoil[1], sizeof(uint8_t));
				VirtualProtect(NoRecoilAddress, sizeof(uint8_t), old, NULL);
			}

			if (cfg_AimbotRapidFire)
			{
				DWORD old;
				VirtualProtect(RapidFireAddress, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
				memcpy(RapidFireAddress, &RapidFire[1], sizeof(uint8_t));
				VirtualProtect(RapidFireAddress, sizeof(uint8_t), old, NULL);
			}


			isGUIInit = true;
		}

		if (updateMiddle)
		{
			TFD_SDK::UCanvas* myCanvas = static_cast<TFD_SDK::UCanvas*>(Canvas);
			float aspectRatioX = myCanvas->SizeX / myCanvas->ClipX;
			float aspectRatioY = myCanvas->SizeY / myCanvas->ClipY;

			ScreenMiddle.X = (myCanvas->SizeX / 2.0f) / aspectRatioX;
			ScreenMiddle.Y = (myCanvas->SizeY / 2.0f) / aspectRatioY;
			updateMiddle = false;
		}

		if (IsKeyPressed(VK_INSERT))
		{
			cfg_DrawMenu = !cfg_DrawMenu;
			if (!cfg_DrawMenu)
				SaveCFG();
		}

		UpdateControllerState();


		if (CheckPointers())
		{

			int State = -1;
			if (GWorld && GWorld->OwningGameInstance && GWorld->OwningGameInstance->IsA(TFD_SDK::UM1GameInstance::StaticClass()))
			{
				State = (int)(static_cast<TFD_SDK::UM1GameInstance*>(GWorld->OwningGameInstance)->ConnectionState);
			}

			if (State != 10) // Game isn't ready, don't do anything or it will likely crash
				return M1org(self, Canvas);

			if (inGame && PlayerIngameController)
			{
				if (PlayerIngameController->HeartbeatTesterComponent && PlayerIngameController->HeartbeatTesterComponent->IsActive())
					PlayerIngameController->HeartbeatTesterComponent->Deactivate();
			}



			//This code is used to find the BoneArray offset in case it moves, but it can also be moved because the parent classes have shifted
			//if (IsKeyPressed(VK_NUMPAD1))
			//{
			//	//int off = offsetof(TFD_SDK::USkinnedMeshComponent, BoneArray);
			//	//std::cout << off << "\n";

			//	BoneOffset += 0x04;
			//	//auto pp = std::addressof(PlayerController->Character->Mesh) + 0x04;
			//	TFD_SDK::USkinnedMeshComponent* SkinnedMeshComp = static_cast<TFD_SDK::USkinnedMeshComponent*>(PlayerController->Character->Mesh);

			//	// Step 2: Add the offset to the base address
			//	uintptr_t SkinnedMeshBaseAddress = reinterpret_cast<uintptr_t>(SkinnedMeshComp);
			//	uintptr_t TargetAddress = SkinnedMeshBaseAddress + BoneOffset;

			//	// Step 3: Cast the address to a TArray<FTransform> pointer
			//	BoneArrayPointer = reinterpret_cast<TFD_SDK::TArray<TFD_SDK::FTransform>*>(TargetAddress);
			//}
			//if (IsKeyPressed(VK_NUMPAD0))
			//{
			//	BoneOffset -= 0x04;
			//	TFD_SDK::USkinnedMeshComponent* SkinnedMeshComp = static_cast<TFD_SDK::USkinnedMeshComponent*>(PlayerController->Character->Mesh);

			//	// Step 2: Add the offset to the base address
			//	uintptr_t SkinnedMeshBaseAddress = reinterpret_cast<uintptr_t>(SkinnedMeshComp);
			//	uintptr_t TargetAddress = SkinnedMeshBaseAddress + BoneOffset;

			//	// Step 3: Cast the address to a TArray<FTransform> pointer
			//	BoneArrayPointer = reinterpret_cast<TFD_SDK::TArray<TFD_SDK::FTransform>*>(TargetAddress);
			//}
			//char bufferlol[32];
			//sprintf_s(bufferlol, "BONEARRAY: %02X", BoneOffset);
			//ZeroGUI::TextLeft((char*)bufferlol, TFD_SDK::FVector2D{ 280, 250 }, ColorRed, false);
			//if (BoneArrayPointer && BoneArrayPointer->IsValid())
			//{
			//	ZeroGUI::TextLeft((char*)"Valid!?", TFD_SDK::FVector2D{ 280, 250 + 12 }, ColorGold, false);
			//	char bufferlol2[64];
			//	sprintf_s(bufferlol2, "Num: %d - Max: %d - Number of Bones: %d", BoneArrayPointer->Num(), BoneArrayPointer->Max(), PlayerController->Character->Mesh->GetNumBones());
			//	ZeroGUI::TextLeft((char*)bufferlol2, TFD_SDK::FVector2D{ 280, 250 + 24 }, ColorGold, false);
			//	if (BoneArrayPointer->Num() == 316)
			//	{
			//		TFD_SDK::FMatrix ComponentMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(PlayerController->Character->Mesh->K2_GetComponentToWorld());
			//		for (int j = 0; j < BoneArrayPointer->Num(); j++)
			//		{
			//			if (BoneArrayPointer->IsValidIndex(j))
			//			{
			//				TFD_SDK::FTransform bone = BoneArrayPointer->operator[](j);
			//				TFD_SDK::FMatrix BoneMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(bone);
			//				TFD_SDK::FMatrix WorldMatrix = TFD_SDK::UKismetMathLibrary::Multiply_MatrixMatrix(BoneMatrix, ComponentMatrix);
			//				TFD_SDK::FTransform WorldPosition = TFD_SDK::UKismetMathLibrary::Conv_MatrixToTransform(WorldMatrix);
			//				TFD_SDK::FVector2D BoneScreenPos = { -1, -1 };
			//				if (WorldToScreen(WorldPosition.Translation, &BoneScreenPos))
			//				{
			//					ZeroGUI::TextCenter((char*)"Bone", BoneScreenPos, TFD_SDK::FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, true);
			//					std::cout << offsetof(TFD_SDK::USkinnedMeshComponent, BoneArray) << "\n";
			//				}
			//			}
			//		}
			//	}
			//}

			if (IsKeyPressed(cfg_LootVacuumKey))
				cfg_LootVacuum = !cfg_LootVacuum;

			if (cfg_EnableAimbotToggle && IsKeyPressed(cfg_AimbotToggleKey))
				cfg_EnableAimbot = !cfg_EnableAimbot;

			if (IsKeyPressed(cfg_HotSwapKey))
			{
				if (HotSwapCharacters[HotSwapIndex] != 0)
				{
					TFD_SDK::FM1TemplateId id = { HotSwapCharacters[HotSwapIndex] };
					PlayerController->PrivateOnlineServiceComponent->ServerChangePlayer(id);
				}
			}
			static bool timeEnable = false;
			static bool isTimeHeld = false;
			if (!timeEnable && IsKeyHeld(cfg_TimeScaleHoldKey))
			{
				if (!isTimeHeld)
				{
					GWorld->PersistentLevel->WorldSettings->TimeDilation = cfg_TimeScale;
					isTimeHeld = true;
				}
			}
			if (isTimeHeld && !IsKeyHeld(cfg_TimeScaleHoldKey))
			{
				GWorld->PersistentLevel->WorldSettings->TimeDilation = 1.0f;
				isTimeHeld = false;
			}
			if (IsKeyPressed(cfg_TimeScaleKey))
			{
				timeEnable = !timeEnable;
				if (timeEnable)
					GWorld->PersistentLevel->WorldSettings->TimeDilation = cfg_TimeScale;
				else
					GWorld->PersistentLevel->WorldSettings->TimeDilation = 1.0f;
			}

			if (IsKeyPressed(VK_PRIOR))
			{
				if (HotSwapIndex == 3)
					HotSwapIndex = 0;
				else
					HotSwapIndex += 1;
			}
			if (IsKeyPressed(VK_NEXT))
			{
				if (HotSwapIndex == 0)
					HotSwapIndex = 3;
				else
					HotSwapIndex -= 1;
			}

			if (IsKeyPressed(cfg_InstantInfilKey))
			{
				InstantInfiltration();
			}

			if (cfg_HotSwapOverlay)
			{
				for (int i = 0; i < 4; i++)
				{
					char buffer[12];
					sprintf_s(buffer, "%d", HotSwapCharacters[i]);
					if (i == HotSwapIndex)
						ZeroGUI::TextLeft((char*)buffer, TFD_SDK::FVector2D{ 250, 25.0f + (12.0f * i) }, ColorRed, false);
					else
						ZeroGUI::TextLeft((char*)buffer, TFD_SDK::FVector2D{ 250, 25.0f + (12.0f * i) }, ColorWhite, false);
				}
			}
			// This code is for testing controller input detection
			/*char sz[4][1024];
			for (DWORD i = 0; i < 4; i++)
			{
				if (g_Controllers[i].bConnected)
				{
					WORD wButtons = g_Controllers[i].state.Gamepad.wButtons;
					sprintf_s(sz[i],
						"Controller %u: Connected\n Buttons: %s%s%s%s%s%s%s%s%s%s%s%s%s%s\n Left Trigger: %u\n Right Trigger: %u\n Left Thumbstick: %d/%d\n Right Thumbstick: %d/%d",
						i,
						(wButtons& XINPUT_GAMEPAD_DPAD_UP) ? "DPAD_UP " : "",
						(wButtons& XINPUT_GAMEPAD_DPAD_DOWN) ? "DPAD_DOWN " : "",
						(wButtons& XINPUT_GAMEPAD_DPAD_LEFT) ? "DPAD_LEFT " : "",
						(wButtons& XINPUT_GAMEPAD_DPAD_RIGHT) ? "DPAD_RIGHT " : "",
						(wButtons& XINPUT_GAMEPAD_START) ? "START " : "",
						(wButtons& XINPUT_GAMEPAD_BACK) ? "BACK " : "",
						(wButtons& XINPUT_GAMEPAD_LEFT_THUMB) ? "LEFT_THUMB " : "",
						(wButtons& XINPUT_GAMEPAD_RIGHT_THUMB) ? "RIGHT_THUMB " : "",
						(wButtons& XINPUT_GAMEPAD_LEFT_SHOULDER) ? "LEFT_SHOULDER " : "",
						(wButtons& XINPUT_GAMEPAD_RIGHT_SHOULDER) ? "RIGHT_SHOULDER " : "",
						(wButtons& XINPUT_GAMEPAD_A) ? "A " : "",
						(wButtons& XINPUT_GAMEPAD_B) ? "B " : "",
						(wButtons& XINPUT_GAMEPAD_X) ? "X " : "",
						(wButtons& XINPUT_GAMEPAD_Y) ? "Y " : "",
						g_Controllers[i].state.Gamepad.bLeftTrigger,
						g_Controllers[i].state.Gamepad.bRightTrigger,
						g_Controllers[i].state.Gamepad.sThumbLX,
						g_Controllers[i].state.Gamepad.sThumbLY,
						g_Controllers[i].state.Gamepad.sThumbRX,
						g_Controllers[i].state.Gamepad.sThumbRY);
				}
				else
					sprintf_s(sz[i], "Controller %u: Not connected", i);
				ZeroGUI::TextLeft((char*)sz[i], TFD_SDK::FVector2D{ 250.0f + (100.0f * i), 150.0f }, ColorRed, false);
			}*/


			if (cfg_EnablePlayerEnemyESP)
			{
				PlayerEnemyESP();
			}

			if (cfg_EnableItemESP || cfg_LootVacuum)
			{
				ItemESPVacuum();
			}

			if (cfg_EnableAimbotHold || cfg_EnableAimbotToggle)
			{
				Aimbot();
			}

			if (cfg_NoReload)
			{
				InstantReload();
			}

			if (cfg_CacheEnemyNames && NamesChanged)
			{
				WriteEnemyNamesData();
				NamesChanged = false;
			}
			if (cfg_CacheEnemyBones && BonesChanged)
			{
				WriteEnemyBonesData();
				BonesChanged = false;
			}

			// This code was my only attempt at using the teleport, but it just teleports to yourself because the deproject hits the player lol
			/*if (IsKeyPressed(0x58))
			{
				TFD_SDK::FVector Pos, Rot = { 0, 0, 0 };
				TFD_SDK::FVector2D Screen = { ZeroGUI::canvas->SizeX / 2.0f, ZeroGUI::canvas->SizeY / 2.0f };
				if (ZeroGUI::canvas->SizeX > 1920)
				{
					Screen.X *= (1920.0f / ZeroGUI::canvas->SizeX);
					Screen.Y *= (1080.0f / ZeroGUI::canvas->SizeY);
				}
				if (PlayerController->DeprojectScreenPositionToWorld(Screen.X, Screen.Y, &Pos, &Rot))
				{
					TFD_SDK::FRotator Rotation = LocalCharacter->K2_GetActorRotation();
					LocalCharacter->RequestTeleportAtSequence(Pos, Rotation);
				}
			}*/


			if (cfg_DrawMenu)
			{
				DrawMenu();
			}

			ZeroGUI::Draw_Cursor(cfg_DrawMenu);


		}
	}
	return M1org(self, Canvas);
}

void InstantReload()
{
	if (!LocalCharacter)
		return;
	if (!LocalCharacter->IsA(TFD_SDK::AM1Player::StaticClass()))
		return;

	static bool foundWeapon = false;
	static TFD_SDK::UM1WeaponSlotControlComponent* WeaponSlot = nullptr;
	if (!foundWeapon)
	{
		for (int i = 0; i < TFD_SDK::UObject::GObjects->Num(); i++)
		{
			TFD_SDK::UObject* Obj = TFD_SDK::UObject::GObjects->GetByIndex(i);

			if (!Obj)
				continue;

			if (Obj->Flags & TFD_SDK::EObjectFlags::BeginDestroyed ||
				Obj->Flags & TFD_SDK::EObjectFlags::BeingRegenerated ||
				Obj->Flags & TFD_SDK::EObjectFlags::FinishDestroyed ||
				Obj->Flags & TFD_SDK::EObjectFlags::NeedInitialization ||
				Obj->Flags & TFD_SDK::EObjectFlags::WillBeLoaded)
				continue;

			if (Obj->IsA(TFD_SDK::UM1WeaponSlotControlComponent::StaticClass()))
			{
				TFD_SDK::UM1WeaponSlotControlComponent* Wep = static_cast<TFD_SDK::UM1WeaponSlotControlComponent*>(Obj);
				if (!Wep)
					continue;
				if (!Wep->Player_Owner)
					continue;
				if (Wep->Player_Owner == LocalCharacter)
				{
					WeaponSlot = Wep;
					foundWeapon = true;
					break;
				}
			}
		}
	}
	else
	{
		if (!WeaponSlot || WeaponSlot->Player_Owner != LocalCharacter)
		{
			foundWeapon = false;
			return;
		}
		else
		{
			if (WeaponSlot->Flags & TFD_SDK::EObjectFlags::BeginDestroyed ||
				WeaponSlot->Flags & TFD_SDK::EObjectFlags::BeingRegenerated ||
				WeaponSlot->Flags & TFD_SDK::EObjectFlags::FinishDestroyed ||
				WeaponSlot->Flags & TFD_SDK::EObjectFlags::NeedInitialization ||
				WeaponSlot->Flags & TFD_SDK::EObjectFlags::WillBeLoaded)
			{
				foundWeapon = false;
				WeaponSlot = nullptr;
				return;
			}
			if (WeaponSlot->ActivatedWeaponSlot.WeaponSlot.Weapon)
			{
				if (WeaponSlot->ActivatedWeaponSlot.WeaponSlot.Weapon->RoundsComponent)
				{
					if (WeaponSlot->ActivatedWeaponSlot.WeaponSlot.Weapon->RoundsComponent->CurrentRounds < 3)
					{
						WeaponSlot->ActivatedWeaponSlot.WeaponSlot.Weapon->RoundsComponent->ClientFillCurrentRoundByServer();
					}
				}
			}
		}
	}
}

void PlayerEnemyESP()
{
	if (!GWorld || !PlayerController || !PlayerController->ActorManager_Subsystem || !Actors || !Actors->Characters)
		return;

	if (cfg_DrawPlayerNames || cfg_DrawPlayerBoxes || cfg_DrawEnemyNames || cfg_DrawEnemyBoxes || cfg_DrawPlayerLines || cfg_DrawEnemyLines)
	{
		if (Actors->Characters.IsValid() && Actors->Characters.Num() > 0)
		{
			int StartNumber = Actors->Characters.Num();
			for (int i = 0; i < Actors->Characters.Num(); i++)
			{
				if (!GWorld || !PlayerController || !PlayerController->ActorManager_Subsystem || !Actors || !Actors->Characters)
					return;
				if (Actors->Characters.Num() != StartNumber)
					return;
				if (!Actors->Characters.IsValidIndex(i))
					continue;
				TFD_SDK::AM1Character* p = Actors->Characters[i];
				if (p)
				{
					if (p->IsDead())
						continue;
					if (p->IsA(TFD_SDK::AM1Player::StaticClass()))
					{
						TFD_SDK::AM1Player* player = static_cast<TFD_SDK::AM1Player*>(p);
						//if (player->bPlayerInputEnabled)
						//{
						//	continue;
						//}
						TFD_SDK::FVector2D ScreenPos = { -1, -1 };
						if (WorldToScreen(p->K2_GetActorLocation(), &ScreenPos))
						{
							if (cfg_DrawPlayerNames)
							{
								if (player->PlayerName && player->PlayerName.ToString() != "")
								{
									std::string Name = player->PlayerName.ToString();
									ZeroGUI::TextCenter((char*)Name.c_str(), TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, ColorGreen, false);
								}
							}
							if (cfg_DrawPlayerBoxes)
							{
								float ODistance = p->GetDistanceTo(LocalPlayer->PlayerController->Pawn) / cfg_DistanceScale;
								if (ODistance <= 0)
									ODistance = 1.0f;
								if (cfg_DrawCirclesInstead)
								{
									float AdjustedRadius = (cfg_ESPBox.X / ODistance) * cfg_ScaleFactor;
									AdjustedRadius = (AdjustedRadius < 3.0f) ? 3.0f : (AdjustedRadius > 70.0f) ? 70.0f : AdjustedRadius;
									ZeroGUI::DrawCircle(TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, AdjustedRadius, 64, ColorGreen);
								}
								else
								{
									float BoxWidth = (cfg_ESPBox.X / ODistance) * cfg_ScaleFactor;
									float BoxHeight = (cfg_ESPBox.Y / ODistance) * cfg_ScaleFactor;
									BoxWidth = (BoxWidth < 2.0f) ? 2.0f : (BoxWidth > 65.0f) ? 65.0f : BoxWidth;
									BoxHeight = (BoxHeight < 4.0f) ? 4.0f : (BoxHeight > 90.0f) ? 90.0f : BoxHeight;
									ZeroGUI::DrawRectangle(TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, TFD_SDK::FVector2D{ BoxWidth, BoxHeight }, ColorGreen);
								}
							}
							if (cfg_DrawPlayerLines)
								ZeroGUI::DrawActorLine(TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, ColorGreen);
						}

					}
					else if (p->IsA(TFD_SDK::AM1Monster::StaticClass()) || p->CharacterAttribute->IsA(TFD_SDK::UM1MonsterAttribute::StaticClass()))
					{
						TFD_SDK::AM1Monster* monster = static_cast<TFD_SDK::AM1Monster*>(p);
						TFD_SDK::FVector2D ScreenPos = { -1, -1 };
						if (WorldToScreen(p->K2_GetActorLocation(), &ScreenPos))
						{
							TFD_SDK::FLinearColor color = ColorRed;
							if (PlayerController->LineOfSightTo(p, TFD_SDK::FVector{ 0, 0, 0 }, false))
								color = ColorDarkRed;

							// Finding fucking new bone targets
							//if (p->Mesh && p->Mesh->BoneArray.IsValid() && p->Mesh->BoneArray.Num() > 0)
							//{
							//	TFD_SDK::FMatrix ComponentMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(p->Mesh->K2_GetComponentToWorld());
							//	//std::vector<int> indexes = IDBoneMap[p->CharacterId.ID];
							//	for (int j = 0; j < p->Mesh->BoneArray.Num(); j++)
							//	{
							//		if (p->Mesh->BoneArray.IsValidIndex(j))
							//		{
							//			TFD_SDK::FTransform bone = p->Mesh->BoneArray[j];
							//			TFD_SDK::FMatrix BoneMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(bone);
							//			TFD_SDK::FMatrix WorldMatrix = TFD_SDK::UKismetMathLibrary::Multiply_MatrixMatrix(BoneMatrix, ComponentMatrix);
							//			TFD_SDK::FTransform WorldPosition = TFD_SDK::UKismetMathLibrary::Conv_MatrixToTransform(WorldMatrix);
							//			TFD_SDK::FVector2D BoneScreenPos = { -1, -1 };
							//			if (WorldToScreen(WorldPosition.Translation, &BoneScreenPos))
							//			{
							//				ZeroGUI::TextCenter((char*)p->Mesh->GetBoneName(j).ToString().c_str(), BoneScreenPos, TFD_SDK::FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, true);
							//			}
							//		}
							//	}
							//}


							// Was used to detect 'monster' actors
							/*if (monster->Children.Num() > 0)
							{
								for (int a = 0; a < monster->Children.Num(); a++)
								{
									if (monster->Children[a]->IsA(TFD_SDK::AM1AbilityActor::StaticClass()))
									{
										TFD_SDK::FVector2D ScreenPos = { -1, -1 };
										if (WorldToScreen(monster->Children[a]->K2_GetActorLocation(), &ScreenPos))
										{
											ZeroGUI::TextCenter((char*)monster->Children[a]->Class->GetFullName().c_str(), TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y - 50 }, ColorGreen, false);
										}
									}
								}


							}*/


							if (cfg_DrawEnemyNames)
							{
								// This draws ALL of the bone names for ALL enemies
								//	for (int j = 0; j < p->Mesh->BoneArray.Num(); j++)
								//	{
								//		TFD_SDK::FMatrix ComponentMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(monster->Mesh->K2_GetComponentToWorld());
								//		TFD_SDK::FTransform bone = monster->Mesh->BoneArray[j];
								//		TFD_SDK::FMatrix BoneMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(bone);
								//		TFD_SDK::FMatrix WorldMatrix = TFD_SDK::UKismetMathLibrary::Multiply_MatrixMatrix(BoneMatrix, ComponentMatrix);
								//		TFD_SDK::FTransform WorldPosition = TFD_SDK::UKismetMathLibrary::Conv_MatrixToTransform(WorldMatrix);
								//		TFD_SDK::FVector2D BoneScreenPos = { -1, -1 };
								//		if (WorldToScreen(WorldPosition.Translation, &BoneScreenPos))
								//		{
								//			ZeroGUI::TextCenter((char*)monster->Mesh->GetBoneName(j).ToString().c_str(), BoneScreenPos, TFD_SDK::FLinearColor{ 1.0f, 0.0f, 1.0f, 1.0f }, false);
								//		}
								//	}
								if (!IDNameMap.contains(p->CharacterId.ID))
								{
									TFD_SDK::UM1UIActorWidget* Base = p->InfoWidgetComponent->ActorWidget.Get();
									if (Base)
									{
										if (Base->IsA(TFD_SDK::UM1UICharacterInfoBase::StaticClass()))
										{
											TFD_SDK::UM1UICharacterInfoBase* Info = static_cast<TFD_SDK::UM1UICharacterInfoBase*>(Base);
											if (Info)
											{
												std::string name = Info->TB_Name->Text.ToString();
												int id = p->CharacterId.ID;
												IDNameMap.insert({ id, name });
												NamesChanged = true;
											}
										}

									}
								}
								else
								{
									ZeroGUI::TextCenter((char*)IDNameMap[(int)p->CharacterId.ID].c_str(), TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, ColorNameRed, false);
								}
							}
							if (cfg_DrawEnemyBoxes)
							{
								float ODistance = p->GetDistanceTo(LocalPlayer->PlayerController->Pawn) / cfg_DistanceScale;
								if (ODistance <= 0)
									ODistance = 1.0f;
									if (cfg_DrawCirclesInstead)
									{
										float AdjustedRadius = (cfg_ESPBox.X / ODistance) * cfg_ScaleFactor;
										AdjustedRadius = (AdjustedRadius < 3.0f) ? 3.0f : (AdjustedRadius > 70.0f) ? 70.0f : AdjustedRadius;
										ZeroGUI::DrawCircle(TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, AdjustedRadius, 64, ColorRed);
									}
									else
									{
										float BoxWidth = (cfg_ESPBox.X / ODistance) * cfg_ScaleFactor;
										float BoxHeight = (cfg_ESPBox.Y / ODistance) * cfg_ScaleFactor;
										BoxWidth = (BoxWidth < 2.0f) ? 2.0f : (BoxWidth > 65.0f) ? 65.0f : BoxWidth;
										BoxHeight = (BoxHeight < 4.0f) ? 4.0f : (BoxHeight > 90.0f) ? 90.0f : BoxHeight;
										ZeroGUI::DrawRectangle(TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, TFD_SDK::FVector2D{ BoxWidth, BoxHeight }, ColorRed);
									}
							}
							if (cfg_DrawEnemyLines)
								ZeroGUI::DrawActorLine(TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, color);
						}
					}
				}
			}
		}
	}
}
void ItemESPVacuum()
{
	if (cfg_DrawItemBoxes || cfg_DrawItemNames || cfg_DrawItemLines || cfg_LootVacuum || cfg_DrawVaults || cfg_DrawLootBoxes || cfg_DrawLootBarrels)
	{
		if (GWorld->Levels.IsValid())
		{
			for (int i = 0; i < GWorld->Levels.Num(); ++i)
			{
				if (GWorld->Levels.IsValidIndex(i))
				{
					TFD_SDK::ULevel* Level = GWorld->Levels[i];
					if (!Level) continue;

					for (int j = 0; j < Level->Actors.Num(); ++j)
					{
						TFD_SDK::AActor* Actor = (TFD_SDK::AActor*)Level->Actors[j];
						if (!Actor)
							continue;
						if (cfg_DrawVaults)
						{
							if (Actor->IsA(TFD_SDK::AM1FieldInteractableActorMiniGame::StaticClass()))
							{
								TFD_SDK::FVector2D ScreenPosa = { -1, -1 };
								TFD_SDK::FVector WorldPositiona = Actor->K2_GetActorLocation();
								static_cast<TFD_SDK::AM1FieldInteractableActorMiniGame*>(Actor)->MiniGameDifficulty = TFD_SDK::EM1MiniGameDifficulty::Normal;
								if (WorldToScreen(WorldPositiona, &ScreenPosa))
								{
									ZeroGUI::TextCenter((char*)"Coded Vault", TFD_SDK::FVector2D{ ScreenPosa.X, ScreenPosa.Y + 20 }, TFD_SDK::FLinearColor{ 0.9f, 0.0f, 0.9f, 0.9f }, false);
								}
								continue;
							}
						}
						if (cfg_DrawLootBoxes)
						{
							if (Actor->IsA(TFD_SDK::AM1FieldInteractableActor_Hit::StaticClass()))
							{
								if (Actor->GetFullName().contains("VulgusBox"))
								{
									TFD_SDK::FVector2D ScreenPos = { -1, -1 };
									TFD_SDK::FVector WorldPosition = Actor->K2_GetActorLocation();
									if (WorldToScreen(WorldPosition, &ScreenPos))
									{
										ZeroGUI::TextCenter((char*)"LOOT", TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y - 1 }, ColorPink, true);
									}
								}
								continue;
							}
						}
						if (cfg_DrawLootBarrels)
						{
							if (Actor->IsA(TFD_SDK::AM1FieldInteractableActor_Interaction::StaticClass()))
							{
								if (Actor->GetFullName().contains("MilitarySupplies"))
								{
									TFD_SDK::FVector2D ScreenPos = { -1, -1 };
									TFD_SDK::FVector WorldPosition = Actor->K2_GetActorLocation();
									if (WorldToScreen(WorldPosition, &ScreenPos))
									{
										ZeroGUI::TextCenter((char*)"Barrel", TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y - 2 }, ColorSadPink, true);
									}
								}
								continue;
							}
						}

						if (!Actor->IsA(TFD_SDK::AM1DroppedItem::StaticClass()))
							continue;

						TFD_SDK::AM1DroppedItem* Item = static_cast<TFD_SDK::AM1DroppedItem*>(Actor);
						if (Item->IsObtained() || Item->bBeingPickedLocally || Item->bTriedSetToObtained || Item->bObtainRequestedOnClient)
							continue;

						TFD_SDK::FVector2D ScreenPos = { -1, -1 };
						TFD_SDK::FVector WorldPosition = Actor->K2_GetActorLocation();

						if (cfg_LootVacuum)
						{
							TFD_SDK::FVector player = LocalCharacter->K2_GetActorLocation();
							float Distance = WorldPosition.GetDistanceTo(player);
							if (Distance > 150 && Distance < cfg_LootVacuumRange)
								Item->K2_SetActorLocation(player, false, nullptr, true);
						}

						if (WorldToScreen(WorldPosition, &ScreenPos))
						{
							TFD_SDK::FLinearColor color = ColorGrey;
							std::string Text = "Unknown";
							switch (Item->DropItemInfo.ItemBox.Type)
							{
							case TFD_SDK::EM1ItemType::InstantUse:
							{
								if (Item->IsA(TFD_SDK::ABP_AmmoEnhancedDroppedItem_C::StaticClass()))
									Text = "Enhanced Ammo";
								else if (Item->IsA(TFD_SDK::ABP_AmmoGeneralDroppedItem_C::StaticClass()))
									Text = "General Ammo";
								else if (Item->IsA(TFD_SDK::ABP_AmmoHighpowerDroppedItem_C::StaticClass()))
									Text = "Highpower Ammo";
								else if (Item->IsA(TFD_SDK::ABP_AmmoImpactDroppedItem_C::StaticClass()))
									Text = "Impact Ammo";
								else if (Item->IsA(TFD_SDK::ABP_HealthOrbDroppedItem_C::StaticClass()))
								{
									color = HealthColor;
									Text = "Health";
								}
								else if (Item->IsA(TFD_SDK::ABP_ManaOrbDroppedItem_C::StaticClass()))
								{
									color = ManaColor;
									Text = "Mana";
								}
								break;
							}
							case TFD_SDK::EM1ItemType::Equipment:
							{
								if (Item->IsA(TFD_SDK::ABP_EquipTier01DroppedItem_C::StaticClass()))
								{
									color = Tier1Color;
									Text = "T1 Item";
								}
								else if (Item->IsA(TFD_SDK::ABP_EquipTier02DroppedItem_C::StaticClass()))
								{
									color = Tier2Color;
									Text = "T2 Item";
								}
								else if (Item->IsA(TFD_SDK::ABP_EquipTier03DroppedItem_C::StaticClass()))
								{
									color = Tier3Color;
									Text = "T3 Item";
								}
								break;
							}
							case TFD_SDK::EM1ItemType::Consumable:
							{
								if (Item->IsA(TFD_SDK::ABP_ResourceTier01DroppedItem_C::StaticClass()))
								{
									color = Tier1Color;
									Text = "T1 Resource";
								}
								else if (Item->IsA(TFD_SDK::ABP_ResourceTier02DroppedItem_C::StaticClass()))
								{
									color = Tier2Color;
									Text = "T2 Resource";
								}
								else if (Item->IsA(TFD_SDK::ABP_ResourceTier03DroppedItem_C::StaticClass()))
								{
									color = Tier3Color;
									Text = "T3 Resource";
								}
								break;
							}
							case TFD_SDK::EM1ItemType::Rune:
							{
								if (Item->IsA(TFD_SDK::ABP_RuneTier01DroppedItem_C::StaticClass()))
								{
									color = Tier1Color;
									Text = "T1 Rune";
								}
								else if (Item->IsA(TFD_SDK::ABP_RuneTier02DroppedItem_C::StaticClass()))
								{
									color = Tier2Color;
									Text = "T2 Rune";
								}
								else if (Item->IsA(TFD_SDK::ABP_RuneTier03DroppedItem_C::StaticClass()))
								{
									color = Tier3Color;
									Text = "T3 Rune";
								}
								else if (Item->IsA(TFD_SDK::ABP_RuneTier04DroppedItem_C::StaticClass()))
								{
									color = Tier4Color;
									Text = "T4 Rune";
								}
								break;
							}
							case TFD_SDK::EM1ItemType::Currency:
							{
								if (Item->IsA(TFD_SDK::ABP_GoldDroppedItem_C::StaticClass()))
								{
									color = GoldColor;
									Text = "Gold";
								}
								else if (Item->IsA(TFD_SDK::ABP_KuiperShardDroppedItem_C::StaticClass()))
								{
									color = KuiperColor;
									Text = "Kuiper";
								}
								break;
							}
							default:
							{
								if (Item->IsA(TFD_SDK::ABP_BuffOrbDroppedItem_C::StaticClass()))
									Text = "Buff Orb";
								else if (Item->IsA(TFD_SDK::ABP_EmberDroppedItem_C::StaticClass()))
									Text = "Ember";
								break;
							}

							}

							if (cfg_DrawItemNames)
							{
								// Idk what the fuck these items are and I don't think I've ever even seen them in-game
								if (Text == "Unknown" || Text == "Buff Orb" || Text == "Ember")
								{
									int type = (int)Item->DropItemInfo.ItemBox.Type;
									char buffer[32];
									sprintf_s(buffer, "Team Buff %d", type);
									ZeroGUI::TextCenter(buffer, TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y + 20 }, TFD_SDK::FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
								}
								ZeroGUI::TextCenter((char*)Text.c_str(), TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, color, true);
							}

							if (cfg_DrawItemBoxes)
							{
								float ODistance = Item->GetDistanceTo(LocalPlayer->PlayerController->Pawn) / cfg_DistanceScale;
								if (ODistance <= 0)
									ODistance = 1.0f;
								if (cfg_DrawCirclesInstead)
								{
									float AdjustedRadius = (cfg_ESPBox.X / ODistance) * cfg_ScaleFactor;
									AdjustedRadius = (AdjustedRadius < 2.0f) ? 2.0f : (AdjustedRadius > 8.0f) ? 8.0f : AdjustedRadius;
									ZeroGUI::DrawCircle(TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, AdjustedRadius, 64, color);
								}
								else
								{
									float BoxWidth = (cfg_ESPBox.X / ODistance) * cfg_ScaleFactor;
									float BoxHeight = (cfg_ESPBox.Y / ODistance) * cfg_ScaleFactor;
									BoxWidth = (BoxWidth < 2.0f) ? 2.0f : (BoxWidth > 8.0f) ? 8.0f : BoxWidth;
									BoxHeight = (BoxHeight < 2.0f) ? 2.0f : (BoxHeight > 8.0f) ? 8.0f : BoxHeight;
									ZeroGUI::DrawRectangle(TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, TFD_SDK::FVector2D{ BoxWidth, BoxHeight }, color);
								}
							}
							if (cfg_DrawItemLines)
								ZeroGUI::DrawActorLine(TFD_SDK::FVector2D{ ScreenPos.X, ScreenPos.Y }, color);

						}
					}

				}

			}
		}

	}
}

void InstantInfiltration()
{
	TFD_SDK::AM1PlayerState* PlayerStateAM1 = static_cast<TFD_SDK::AM1PlayerState*> (PlayerState);
	if (PlayerStateAM1 && PlayerStateAM1->MissionControlComponent) {
		TFD_SDK::UM1MissionControlComponent* MCC = PlayerStateAM1->MissionControlComponent;
		if (MCC->ActivatedMissions.Num() > 0) {
			for (TFD_SDK::AM1MissionActor* MissionActor : MCC->ActivatedMissions) {
				if (!MissionActor || MissionActor->TaskLinks.Num() == 0) continue;
				//TFD_SDK::UM1DataMission* MissionData = MissionActor->MissionData;
				//TFD_SDK::TArray<TFD_SDK::FM1MissionTaskLink> TaskLinks = MissionActor->TaskLinks;
				TFD_SDK::AM1MissionTaskActor* TaskActor = MissionActor->TaskLinks[0].InstancedTaskActor;
				if (!TaskActor) continue;
				//TFD_SDK::TArray<TFD_SDK::UM1MissionTaskService*> MCCSubs = MCC->SubServices;
				for (TFD_SDK::UM1MissionTaskService* MCCSub : MCC->SubServices) {
					if (MCCSub && MCCSub->bJoined) {
						TFD_SDK::UM1MissionTaskServiceInteraction* MCCInt = static_cast<TFD_SDK::UM1MissionTaskServiceInteraction*> (MCCSub);
						if (!MCCInt) continue;
						TFD_SDK::AM1MissionTaskActorDestructionVulgusPost* VPost = static_cast<TFD_SDK::AM1MissionTaskActorDestructionVulgusPost*> (TaskActor);
						if (!VPost) continue;
						for (TFD_SDK::AM1MissionTargetInteraction* MissionTarget : VPost->MissionTargets) {
							if (MissionTarget) {
								MCCInt->ServerRequestMissionTargetBeginInteraction(MissionTarget, PlayerIngameController);
								Sleep(500);
							}
						}
						break;
					}
				}
				Sleep(1500);

			}
		}
	}
}

HRESULT UpdateControllerState()
{
	DWORD dwResult;
	for (DWORD i = 0; i < 4; i++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &g_Controllers[i].state);

		if (dwResult == ERROR_SUCCESS)
			g_Controllers[i].bConnected = true;
		else
			g_Controllers[i].bConnected = false;
	}

	return S_OK;
}


void Aimbot()
{
	static int currentTargetID;

	if (cfg_EnableAimbotHold)
	{
		if (!cfg_AimbotController && !IsKeyHeld(cfg_AimbotHoldKey))
		{
			currentTargetID = 0;
			Aimbot_BoneIndex = -1;
			return;
		}
		if (cfg_AimbotController)
		{
			bool isAiming = false;
			for (DWORD i = 0; i < 4; i++)
			{
				if (g_Controllers[i].bConnected)
				{
					if (g_Controllers[i].state.Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
					{
						isAiming = true;
					}
				}
			}
			if (!isAiming)
			{
				currentTargetID = 0;
				Aimbot_BoneIndex = -1;
				return;
			}
		}
	}
	else if (cfg_EnableAimbotToggle && !cfg_EnableAimbot)
	{
		currentTargetID = 0;
		Aimbot_BoneIndex = -1;
		return;
	}

	if (currentTargetID && Aimbot_Target && Aimbot_BoneIndex != -1)
	{
		if (Aimbot_Target->IsA(TFD_SDK::AM1Character::StaticClass()))
		{
			TFD_SDK::AM1Character* AimTarget = static_cast<TFD_SDK::AM1Character*>(Aimbot_Target);
			if (!AimTarget->IsDead() && PlayerController->LineOfSightTo(AimTarget, TFD_SDK::FVector{ 0, 0, 0 }, false))
			{
				if (AimTarget->Mesh && AimTarget->Mesh->BoneArray.IsValid() && AimTarget->Mesh->BoneArray.IsValidIndex(Aimbot_BoneIndex))
				{
					TFD_SDK::FMatrix ComponentMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(AimTarget->Mesh->K2_GetComponentToWorld());
					TFD_SDK::FTransform bone = AimTarget->Mesh->BoneArray[Aimbot_BoneIndex];
					TFD_SDK::FMatrix BoneMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(bone);
					TFD_SDK::FMatrix WorldMatrix = TFD_SDK::UKismetMathLibrary::Multiply_MatrixMatrix(BoneMatrix, ComponentMatrix);
					TFD_SDK::FTransform WorldPosition = TFD_SDK::UKismetMathLibrary::Conv_MatrixToTransform(WorldMatrix);
					TFD_SDK::FRotator Angles = TFD_SDK::UKismetMathLibrary::FindLookAtRotation(PlayerController->PlayerCameraManager->GetCameraLocation(), WorldPosition.Translation);
					PlayerController->SetControlRotation(
						TFD_SDK::UKismetMathLibrary::RInterpTo(PlayerController->PlayerCameraManager->GetCameraRotation(), Angles, ((TFD_SDK::UGameplayStatics*)TFD_SDK::UGameplayStatics::StaticClass())->GetWorldDeltaSeconds(GWorld), cfg_AimbotSmoothing));
					return;
				}

			}
		}
		else if (Aimbot_Target->IsA(TFD_SDK::AM1AbilityActor::StaticClass()))
		{

			TFD_SDK::FRotator Angles = TFD_SDK::UKismetMathLibrary::FindLookAtRotation(PlayerController->PlayerCameraManager->GetCameraLocation(), Aimbot_Target->K2_GetActorLocation());
			PlayerController->SetControlRotation(
				TFD_SDK::UKismetMathLibrary::RInterpTo(PlayerController->PlayerCameraManager->GetCameraRotation(), Angles, ((TFD_SDK::UGameplayStatics*)TFD_SDK::UGameplayStatics::StaticClass())->GetWorldDeltaSeconds(GWorld), cfg_AimbotSmoothing));
			// This draws the class name of the aimbot target
			//ZeroGUI::TextCenter((char*)Aimbot_Target->Class->Super->GetFullName().c_str(), TFD_SDK::FVector2D{ 200, 200 }, TFD_SDK::FLinearColor{ 0.0f, 0.0f, 1.0f, 1.0f }, false);
			return;
		}
		currentTargetID = 0;
		Aimbot_BoneIndex = -1;
		Aimbot_Target = nullptr;
	}

	Aimbot_Target = GetClosestEnemy(currentTargetID);
}

TFD_SDK::AActor* GetClosestEnemy(int& ID)
{

	TFD_SDK::AActor* ret = nullptr;
	float closestDistance = INFINITY;
	Aimbot_BoneIndex = -1;



	if (Actors->Characters.IsValid() && Actors->Characters.Num() > 0)
	{
		int StartNumber = Actors->Characters.Num();
		for (int i = 0; i < Actors->Characters.Num(); i++)
		{
			if (Actors->Characters.Num() != StartNumber)
				return nullptr;
			if (!Actors->Characters.IsValidIndex(i))
				continue;
			TFD_SDK::AM1Character* p = Actors->Characters[i];
			if (p)
			{
				if (p->IsDead())
					continue;
				//if (!p->IsA(TFD_SDK::AM1Monster::StaticClass()) && !p->CharacterAttribute->IsA(TFD_SDK::UM1MonsterAttribute::StaticClass()))
				//if (!p->CharacterAttribute->IsA(TFD_SDK::UM1MonsterAttribute::StaticClass()))
				//	continue;
				if (p->IsA(TFD_SDK::AM1Monster::StaticClass()) || p->CharacterAttribute->IsA(TFD_SDK::UM1MonsterAttribute::StaticClass()))
				{
					if (!PlayerController->LineOfSightTo(p, TFD_SDK::FVector{ 0, 0, 0 }, false))
					{
						if (p->Children.Num() > 0)
						{
							bool any = false;
							for (int c = 0; c < p->Children.Num(); c++)
							{
								if (p->Children[c]->IsA(TFD_SDK::AM1AbilityActor::StaticClass()))
									if (p->Children[c]->Class->GetFullName().contains("Immunity") || p->Children[c]->Class->GetFullName().contains("JudgementEye"))
										if (PlayerController->LineOfSightTo(p->Children[c], TFD_SDK::FVector{ 0, 0, 0 }, false))
											any = true;
							}
							if (!any)
								continue;
						}
						else
							continue;
					}
					if (!IDBoneMap.contains(p->CharacterId.ID))
					{
						if (p->Mesh && p->Mesh->BoneArray.IsValid() && p->Mesh->BoneArray.Num() > 0)
						{
							std::vector<int> bones = { };
							for (int j = 0; j < p->Mesh->BoneArray.Num(); j++)
							{
								if (p->Mesh->BoneArray.IsValidIndex(j))
								{
									if (p->Mesh->GetBoneName(j).ToString().contains("Weakness") || p->Mesh->GetBoneName(j).ToString().contains("-Head") || p->Mesh->GetBoneName(j).ToString().contains("_head"))
									{
										bones.push_back(j);
									}
								}
							}
							if (bones.size() > 0)
							{
								IDBoneMap.insert({ p->CharacterId.ID, bones });
								BonesChanged = true;
							}
						}
					}
					else
					{
						// Some enemies have 'balls' that spawn, they are Children of the current Actor with a class of M1AbilityActor
						// no bones for them, so target just their actor location?
						if (p->Mesh && p->Mesh->BoneArray.IsValid() && p->Mesh->BoneArray.Num() > 0)
						{
							TFD_SDK::FMatrix ComponentMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(p->Mesh->K2_GetComponentToWorld());
							std::vector<int> indexes = IDBoneMap[p->CharacterId.ID];
							for (int j = 0; j < indexes.size(); j++)
							{
								if (p->Mesh->BoneArray.IsValidIndex(indexes[j]))
								{
									TFD_SDK::FTransform bone = p->Mesh->BoneArray[indexes[j]];
									TFD_SDK::FMatrix BoneMatrix = TFD_SDK::UKismetMathLibrary::Conv_TransformToMatrix(bone);
									TFD_SDK::FMatrix WorldMatrix = TFD_SDK::UKismetMathLibrary::Multiply_MatrixMatrix(BoneMatrix, ComponentMatrix);
									TFD_SDK::FTransform WorldPosition = TFD_SDK::UKismetMathLibrary::Conv_MatrixToTransform(WorldMatrix);
									TFD_SDK::FVector2D BoneScreenPos = { -1, -1 };
									if (WorldToScreen(WorldPosition.Translation, &BoneScreenPos))
									{
										//ZeroGUI::TextCenter((char*)"Bone", BoneScreenPos, TFD_SDK::FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, true);

										float distance = TFD_SDK::UKismetMathLibrary::Distance2D(ScreenMiddle, BoneScreenPos);
										//ZeroGUI::TextCenter((char*)p->Class->GetFullName().c_str(), TFD_SDK::FVector2D{ 500, 500 }, TFD_SDK::FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
										if (distance > cfg_AimbotFOV)
											continue;
										if (distance < closestDistance)
										{
											closestDistance = distance;
											ID = p->Index;
											Aimbot_BoneIndex = indexes[j];
											ret = p;
										}
									}
								}
							}
						}

						if (p->Children.Num() > 0)
						{
							for (int a = 0; a < p->Children.Num(); a++)
							{
								/*TFD_SDK::FVector2D ScreenPos = { -1, -1 };
								if (WorldToScreen(p->Children[a]->K2_GetActorLocation(), &ScreenPos))
								{
									ZeroGUI::TextCenter((char*)p->Children[a]->Class->GetFullName().c_str(), ScreenPos, TFD_SDK::FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
								}*/
								if (p->Children[a]->IsA(TFD_SDK::AM1AbilityActor::StaticClass()))
								{
									if (!(p->Children[a]->Class->GetFullName().contains("Immun")) && !(p->Children[a]->Class->GetFullName().contains("JudgementEye")))
										continue;
									TFD_SDK::FVector2D ScreenPos = { -1, -1 };
									if (WorldToScreen(p->Children[a]->K2_GetActorLocation(), &ScreenPos))
									{
										float distance = TFD_SDK::UKismetMathLibrary::Distance2D(ScreenMiddle, ScreenPos);
										//ZeroGUI::TextCenter((char*)p->Children[a]->Class->GetFullName().c_str(), TFD_SDK::FVector2D{ 500.0f, 200.0f + (50.0f * a) }, TFD_SDK::FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
										if (distance > cfg_AimbotFOV)
											continue;
										if (distance < closestDistance)
										{
											closestDistance = distance;
											ID = p->Children[a]->Index;
											Aimbot_BoneIndex = 999;
											ret = p->Children[a];
										}
									}
								}
							}
						}

					}
				}
			}
		}
	}
	return ret;
}

struct FColorSettings {
	float R, G, B, A;
	FColorSettings(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
		: R(r), G(g), B(b), A(a) {
	}
	SDK::FLinearColor ToLinearColor() const {
		return SDK::FLinearColor(R, G, B, A);
	}
};

float menuWidth = 550.0f;
float menuHeight = 600.0f;

void DrawMenu()
{
	if (ZeroGUI::Window((char*)"Descendant Internal - Tivmo Edition", &cfg_WindowPos, TFD_SDK::FVector2D{ menuWidth, menuHeight }, cfg_DrawMenu))
	{
		if (IsKeyPressed(VK_UP)) 
		{
			menuHeight += 10.0f;  // Increase height by 10
		}
		if (IsKeyPressed(VK_DOWN)) 
		{
			menuHeight -= 10.0f;  // Decrease height by 10
		}
		if (IsKeyPressed(VK_RIGHT))
		{
			menuWidth += 10.0f;  // Increase height by 10
		}
		if (IsKeyPressed(VK_LEFT)) 
		{
			menuWidth -= 10.0f;  // Decrease height by 10
		}
		

		static int tab = 0;
		if (ZeroGUI::ButtonTab((char*)"Actors", TFD_SDK::FVector2D{ 75, 20 }, tab == 0)) tab = 0;
		if (ZeroGUI::ButtonTab((char*)"Items", TFD_SDK::FVector2D{ 75, 20 }, tab == 1)) tab = 1;
		if (ZeroGUI::ButtonTab((char*)"Aimbot", TFD_SDK::FVector2D{ 75, 20 }, tab == 2)) tab = 2;
		if (ZeroGUI::ButtonTab((char*)"Extras", TFD_SDK::FVector2D{ 75, 20 }, tab == 3)) tab = 3;
		if (ZeroGUI::ButtonTab((char*)"Colors", TFD_SDK::FVector2D{ 75, 20 }, tab == 4)) tab = 4;
		if (ZeroGUI::ButtonTab((char*)"Credit", TFD_SDK::FVector2D{ 70, 15 }, tab == 5)) tab = 5;
		ZeroGUI::NextColumn(95.0f);

		if (tab == 0)
		{
			ZeroGUI::Checkbox((char*)"Actor ESP", &cfg_EnablePlayerEnemyESP);
			ZeroGUI::Checkbox((char*)"Player Boxes", &cfg_DrawPlayerBoxes);
			ZeroGUI::Checkbox((char*)"Player Names", &cfg_DrawPlayerNames);
			ZeroGUI::Checkbox((char*)"Player Lines", &cfg_DrawPlayerLines);
			ZeroGUI::SliderFloat((char*)"Shape Range-Scale", &cfg_DistanceScale, 500.0f, 5000.0f);
			ZeroGUI::SliderFloat((char*)"Shape Size Factor", &cfg_ScaleFactor, 0.1f, 10.0f);
		}
		if (tab == 1)
		{
			ZeroGUI::Checkbox((char*)"Item ESP", &cfg_EnableItemESP);
			ZeroGUI::Checkbox((char*)"Item Boxes", &cfg_DrawItemBoxes);
			ZeroGUI::Checkbox((char*)"Item Names", &cfg_DrawItemNames);
			ZeroGUI::Checkbox((char*)"Item Lines", &cfg_DrawItemLines);
			ZeroGUI::Text((char*)"Loot Vacuum Toggle:");
			ZeroGUI::SliderFloat((char*)"Loot Vacuum Range", &cfg_LootVacuumRange, 151.0f, 200000.0f);
		}
		if (tab == 2)
		{
			if (ZeroGUI::Checkbox((char*)"Aimbot(Hold)", &cfg_EnableAimbotHold))
			{
				if (cfg_EnableAimbotHold)
					cfg_EnableAimbotToggle = false;
			}
			ZeroGUI::Text((char*)"Aimbot Hold Key:");
			ZeroGUI::Text((char*)"Aimbot Toggle Key:");
			ZeroGUI::Checkbox((char*)"Enable Controller Support", &cfg_AimbotController);
			ZeroGUI::SliderFloat((char*)"Aimbot Screen Distance", &cfg_AimbotFOV, 1.0f, 2000.0f);
			ZeroGUI::SliderFloat((char*)"Aimbot Speed", &cfg_AimbotSmoothing, 1.0f, 1000.0f);
			ZeroGUI::Text((char*)"Instant Infiltration -Unknown Risk- Thank Athrun");
			ZeroGUI::Hotkey((char*)"Instant Infiltration Hotkey", TFD_SDK::FVector2D{ 100, 20 }, &cfg_InstantInfilKey);
		}
		if (tab == 3)
		{
			{
				if (ZeroGUI::Checkbox((char*)"Names Cache", &cfg_CacheEnemyNames) && cfg_CacheEnemyNames)
					WriteEnemyNamesData();
			}
			ZeroGUI::Text((char*)"Timescale Toggle:");
			ZeroGUI::Text((char*)"Timescale Hold:");
			ZeroGUI::SliderFloat((char*)"Timescale:", &cfg_TimeScale, 1.0f, 10.0f);
			ZeroGUI::Text((char*)"Use the PageUp and PageDown keys to change slots.");
			ZeroGUI::Checkbox((char*)"Show Swap Slot Overlay", &cfg_HotSwapOverlay);
			char SlotText[64];
			sprintf_s(SlotText, "Character ID for Slot %d: %d", HotSwapIndex, HotSwapCharacters[HotSwapIndex]);
			ZeroGUI::Text((char*)SlotText);
			if (ZeroGUI::Button((char*)"Set Character Slot", TFD_SDK::FVector2D{ 110, 20 }))
			{
				HotSwapCharacters[HotSwapIndex] = LocalCharacter->CharacterId.ID;
			}
			ZeroGUI::SameLine();
			if (ZeroGUI::Button((char*)"Clear Character Slot", TFD_SDK::FVector2D{ 110, 20 }))
			{
				HotSwapCharacters[HotSwapIndex] = 0;
			}
			ZeroGUI::Text((char*)"Hot Swap Key:");
			ZeroGUI::SameLine();
			ZeroGUI::Hotkey((char*)"Hot Swap Hotkey", TFD_SDK::FVector2D{ 100, 20 }, &cfg_HotSwapKey);
		}
		if (tab == 4)
		{
			ZeroGUI::ColorPicker((char*)"Text", &ZeroGUI::Colors::Text);
			ZeroGUI::ColorPicker((char*)"Text Shadow", &ZeroGUI::Colors::Text_Shadow);
			ZeroGUI::ColorPicker((char*)"Text Outline", &ZeroGUI::Colors::Text_Outline);
			ZeroGUI::ColorPicker((char*)"Button Idle", &ZeroGUI::Colors::Button_Idle);
			ZeroGUI::ColorPicker((char*)"Button Hovered", &ZeroGUI::Colors::Button_Hovered);
			ZeroGUI::ColorPicker((char*)"Button Active", &ZeroGUI::Colors::Button_Active);
			ZeroGUI::ColorPicker((char*)"Checkbox Idle", &ZeroGUI::Colors::Checkbox_Idle);
			ZeroGUI::ColorPicker((char*)"Checkbox Enabled", &ZeroGUI::Colors::Checkbox_Enabled);
			ZeroGUI::ColorPicker((char*)"Health Color", &HealthColor);
			ZeroGUI::ColorPicker((char*)"Tier 1 Color", &Tier1Color);
			ZeroGUI::ColorPicker((char*)"Tier 3 Color", &Tier3Color);
			ZeroGUI::ColorPicker((char*)"Gold Color", &GoldColor);
			ZeroGUI::Text((char*)"Arrow Keys control Menu Size as well.");
		}
		if (tab == 5)
		{
			ZeroGUI::NextColumn(125.0f);
			ZeroGUI::PushNextElementY(70.0f);
			ZeroGUI::Text((char*)"Thanks to EizzilB and Athrun for assisting",true);
			ZeroGUI::Text((char*)"and helping keep this mod up and working.", true);
			ZeroGUI::Text((char*)"Be Careful in Public/With Instant", true);
			ZeroGUI::Text((char*)"Infiltrations. Ive been using this for a", true);
			ZeroGUI::Text((char*)"month now without a ban but I have not used", true);
			ZeroGUI::Text((char*)"the Infiltrations much, so I am unsure", true);
			ZeroGUI::Text((char*)"how safe it is to abuse.", true);
		}
		ZeroGUI::NextColumn(280.0f);
		if (tab == 0)
		{
			ZeroGUI::Checkbox((char*)"Circle ESP", &cfg_DrawCirclesInstead);
			ZeroGUI::Checkbox((char*)"Enemy Boxes", &cfg_DrawEnemyBoxes);
			ZeroGUI::Checkbox((char*)"Enemy Names", &cfg_DrawEnemyNames);
			ZeroGUI::Checkbox((char*)"Enemy Lines", &cfg_DrawEnemyLines);
			ZeroGUI::SliderFloat((char*)"Menu Height-Up/Down Arrow Keys)", &menuHeight, 400.0f, 1000.0f);
			ZeroGUI::SliderFloat((char*)"Menu Width-Left/Right Arrow Keys)", &menuWidth, 400.0f, 1000.0f);
		}
		if (tab == 1)
		{
			ZeroGUI::Checkbox((char*)"Coded Vaults", &cfg_DrawVaults);
			ZeroGUI::Checkbox((char*)"Vulgus Crates", &cfg_DrawLootBoxes);
			ZeroGUI::Checkbox((char*)"Munition Barrels", &cfg_DrawLootBarrels);
			ZeroGUI::Checkbox((char*)"Loot Vacuum", &cfg_LootVacuum);
			ZeroGUI::Hotkey((char*)"Loot Vacuum Hotkey", TFD_SDK::FVector2D{ 100, 20 }, &cfg_LootVacuumKey);
		}
		if (tab == 2)
		{
			if (ZeroGUI::Checkbox((char*)"Enable Aimbot(Toggle)", &cfg_EnableAimbotToggle))
			{
				if (cfg_EnableAimbotToggle)
					cfg_EnableAimbotHold = false;
			}
			ZeroGUI::Hotkey((char*)"Aimbot Hold", TFD_SDK::FVector2D{ 80, 20 }, & cfg_AimbotHoldKey);
			ZeroGUI::Hotkey((char*)"Aimbot Toggle", TFD_SDK::FVector2D{ 80, 20 }, & cfg_AimbotToggleKey);
			ZeroGUI::Text((char*)"(Hold with Left Trigger)");
			if (ZeroGUI::Checkbox((char*)"No Spread", &cfg_AimbotNoSpread))
			{
				if (cfg_AimbotNoSpread)
				{
					DWORD old;
					VirtualProtect(NoSpreadAddress, sizeof(uint8_t) * 9, PAGE_EXECUTE_READWRITE, &old);
					memcpy(NoSpreadAddress, &NoSpreadCheat, sizeof(uint8_t) * 9);
					VirtualProtect(NoSpreadAddress, sizeof(uint8_t) * 9, old, NULL);
				}
				else
				{
					DWORD old;
					VirtualProtect(NoSpreadAddress, sizeof(uint8_t) * 9, PAGE_EXECUTE_READWRITE, &old);
					memcpy(NoSpreadAddress, &NoSpreadOriginal, sizeof(uint8_t) * 9);
					VirtualProtect(NoSpreadAddress, sizeof(uint8_t) * 9, old, NULL);
				}
			}
			if (ZeroGUI::Checkbox((char*)"No Recoil", &cfg_AimbotNoRecoil))
			{
				if (cfg_AimbotNoRecoil)
				{
					DWORD old;
					VirtualProtect(NoRecoilAddress, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
					memcpy(NoRecoilAddress, &Recoil[1], sizeof(uint8_t));
					VirtualProtect(NoRecoilAddress, sizeof(uint8_t), old, NULL);
				}
				else
				{
					DWORD old;
					VirtualProtect(NoRecoilAddress, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
					memcpy(NoRecoilAddress, &Recoil[0], sizeof(uint8_t));
					VirtualProtect(NoRecoilAddress, sizeof(uint8_t), old, NULL);
				}
			}
			if (ZeroGUI::Checkbox((char*)"Rapidfire (Iffy)", &cfg_AimbotRapidFire))
			{
				if (cfg_AimbotRapidFire)
				{
					DWORD old;
					VirtualProtect(RapidFireAddress, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
					memcpy(RapidFireAddress, &RapidFire[1], sizeof(uint8_t));
					VirtualProtect(RapidFireAddress, sizeof(uint8_t), old, NULL);
				}
				else
				{
					DWORD old;
					VirtualProtect(RapidFireAddress, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
					memcpy(RapidFireAddress, &RapidFire[0], sizeof(uint8_t));
					VirtualProtect(RapidFireAddress, sizeof(uint8_t), old, NULL);
				}
			}
			ZeroGUI::Checkbox((char*)"AutoReload", &cfg_NoReload);
		}
		if (tab == 3)
		{
			{
				if (ZeroGUI::Checkbox((char*)"Bones Cache", &cfg_CacheEnemyBones) && cfg_CacheEnemyBones)
					WriteEnemyBonesData();
			}
			ZeroGUI::Hotkey((char*)"Timescale Hotkey:", TFD_SDK::FVector2D{ 100, 20 }, & cfg_TimeScaleKey);
			ZeroGUI::Hotkey((char*)"Timescale Hold Key:", TFD_SDK::FVector2D{ 100, 20 }, & cfg_TimeScaleHoldKey);
		}
		if (tab == 4)
		{
			ZeroGUI::ColorPicker((char*)"Header", &ZeroGUI::Colors::Window_Header);
			ZeroGUI::ColorPicker((char*)"Background", &ZeroGUI::Colors::Window_Background);
			ZeroGUI::ColorPicker((char*)"Slider Button", &ZeroGUI::Colors::Slider_Button);
			ZeroGUI::ColorPicker((char*)"Slider Idle", &ZeroGUI::Colors::Slider_Idle);
			ZeroGUI::ColorPicker((char*)"Slider Hovered", &ZeroGUI::Colors::Slider_Hovered);
			ZeroGUI::ColorPicker((char*)"Slider Progress", &ZeroGUI::Colors::Slider_Progress);
			ZeroGUI::ColorPicker((char*)"Checkbox Hovered", &ZeroGUI::Colors::Checkbox_Hovered);
			ZeroGUI::ColorPicker((char*)"Cursor Color", &ZeroGUI::Colors::CursorColor);
			ZeroGUI::ColorPicker((char*)"Mana Color", &ManaColor);
			ZeroGUI::ColorPicker((char*)"Tier 2 Color", &Tier2Color);
			ZeroGUI::ColorPicker((char*)"Tier 4 Color", &Tier4Color);
			ZeroGUI::ColorPicker((char*)"Kuiper Color", &KuiperColor);
		}
		ZeroGUI::Render();
		ZeroGUI::Draw_Cursor(true);
	}
}

void LoadCFG()
{
	ini.SetUnicode();
	if (ini.LoadFile("cfg.ini") < 0)
	{
		SaveCFG();
	}
	else
	{
		cfg_WindowPos = { (float)ini.GetDoubleValue("Menu", "X"), (float)ini.GetDoubleValue("Menu", "Y") };
		cfg_EnablePlayerEnemyESP = ini.GetBoolValue("ESP", "EnablePlayerEnemyESP");
		cfg_DrawPlayerNames = ini.GetBoolValue("ESP", "EnablePlayerNames");
		cfg_DrawPlayerBoxes = ini.GetBoolValue("ESP", "EnablePlayerBoxes");
		cfg_DrawPlayerLines = ini.GetBoolValue("ESP", "EnablePlayerLines");
		cfg_DrawEnemyNames = ini.GetBoolValue("ESP", "EnableEnemyNames");
		cfg_DrawEnemyBoxes = ini.GetBoolValue("ESP", "EnableEnemyBoxes");
		cfg_DrawEnemyLines = ini.GetBoolValue("ESP", "EnableEnemyLines");
		cfg_DistanceScale = ini.GetDoubleValue("ESP", "ShapeRangeScale"); 
		cfg_ScaleFactor = ini.GetDoubleValue("ESP", "ShapeScaleFactor");

		cfg_EnableItemESP = ini.GetBoolValue("ESP", "EnableItemESP");
		cfg_DrawItemBoxes = ini.GetBoolValue("ESP", "EnableItemCircles");
		cfg_DrawItemNames = ini.GetBoolValue("ESP", "EnableItemNames");
		cfg_DrawItemLines = ini.GetBoolValue("ESP", "EnableItemLines");
		cfg_LootVacuum = ini.GetBoolValue("Extra", "EnableVacuum");
		cfg_LootVacuumRange = ini.GetDoubleValue("Extra", "VacuumRange");
		cfg_LootVacuumKey = (int)ini.GetDoubleValue("Extra", "VacuumKey");
		cfg_DrawVaults = ini.GetBoolValue("ESP", "EnableCodedVaults");
		cfg_DrawLootBoxes = ini.GetBoolValue("ESP", "EnableVulgusResourceCrates");
		cfg_DrawLootBarrels = ini.GetBoolValue("ESP", "EnableMunitionBarrels");
		cfg_DrawCirclesInstead = ini.GetBoolValue("ESP", "EnableDrawCirclesInstead");

		cfg_EnableAimbotHold = ini.GetBoolValue("Aimbot", "EnableAimbotHold");
		cfg_EnableAimbotToggle = ini.GetBoolValue("Aimbot", "EnableAimbotToggle");
		cfg_AimbotHoldKey = (int)ini.GetDoubleValue("Aimbot", "AimbotHoldKey");
		cfg_AimbotToggleKey = (int)ini.GetDoubleValue("Aimbot", "AimbotToggleKey");
		cfg_AimbotController = ini.GetBoolValue("Aimbot", "EnableController");
		cfg_AimbotFOV = ini.GetDoubleValue("Aimbot", "AimbotFOV");
		cfg_AimbotSmoothing = ini.GetDoubleValue("Aimbot", "AimbotSmooth");
		cfg_AimbotNoSpread = ini.GetBoolValue("Aimbot", "EnableNoSpread");
		cfg_NoReload = ini.GetBoolValue("Extra", "EnableNoReload");
		cfg_AimbotRapidFire = ini.GetBoolValue("Aimbot", "EnableRapidFire");
		cfg_AimbotNoRecoil = ini.GetBoolValue("Aimbot", "EnableNoRecoil");
		cfg_InstantInfilKey = (int)ini.GetDoubleValue("Extra", "InstantInfilKey");
		
		cfg_CacheEnemyNames = ini.GetBoolValue("Extra", "CacheNames");
		cfg_CacheEnemyBones = ini.GetBoolValue("Extra", "CacheBones");
		
		cfg_HotSwapKey = (int)ini.GetDoubleValue("Extra", "HotSwapKey");
		cfg_HotSwapOverlay = ini.GetBoolValue("Extra", "HotSwapOverlay");

		HotSwapCharacters[0] = (int)ini.GetDoubleValue("Extra", "HotSwapSlot1", 0);
		HotSwapCharacters[1] = (int)ini.GetDoubleValue("Extra", "HotSwapSlot2", 0);
		HotSwapCharacters[2] = (int)ini.GetDoubleValue("Extra", "HotSwapSlot3", 0);
		HotSwapCharacters[3] = (int)ini.GetDoubleValue("Extra", "HotSwapSlot4", 0);

		cfg_TimeScale = ini.GetDoubleValue("Extra", "Timescale");
		cfg_TimeScaleKey = (int)ini.GetDoubleValue("Extra", "TimescaleKey");
		cfg_TimeScaleHoldKey = (int)ini.GetDoubleValue("Extra", "TimescaleHoldKey");

		HealthColor = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "HealthColorR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "HealthColorG", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "HealthColorB", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "HealthColorA", 1.0f)));
		ManaColor = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "ManaColorR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "ManaColorG", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "ManaColorB", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "ManaColorA", 1.0f)));
		GoldColor = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "GoldColorR", 0.8f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "GoldColorG", 0.8f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "GoldColorB", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "GoldColorA", 0.8f)));
		KuiperColor = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "KuiperColorR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "KuiperColorG", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "KuiperColorB", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "KuiperColorA", 1.0f)));
		Tier1Color = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier1ColorR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier1ColorG", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier1ColorB", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier1ColorA", 1.0f)));
		Tier2Color = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier2ColorR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier2ColorG", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier2ColorB", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier2ColorA", 1.0f)));
		Tier3Color = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier3ColorR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier3ColorG", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier3ColorB", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier3ColorA", 1.0f)));
		Tier4Color = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier4ColorR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier4ColorG", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier4ColorB", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Tier4ColorA", 1.0f)));
		ZeroGUI::Colors::CursorColor = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "CursorColorR", 0.30f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "CursorColorG", 0.30f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "CursorColorB", 0.80f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "CursorColorA", 1.0f)));
		ZeroGUI::Colors::Text = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "TextR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "TextG", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "TextB", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "TextA", 1.0f)));
		ZeroGUI::Colors::Window_Header = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Window_HeaderR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Window_HeaderG", 0.55f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Window_HeaderB", 0.55f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Window_HeaderA", 0.75f)));
		ZeroGUI::Colors::Window_Background = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Window_BackgroundR", 0.05f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Window_BackgroundG", 0.15f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Window_BackgroundB", 0.15f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Window_BackgroundA", 0.5f)));
		ZeroGUI::Colors::Button_Idle = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_IdleR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_IdleG", 0.75f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_IdleB", 0.75f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_IdleA", 0.8f)));
		ZeroGUI::Colors::Button_Hovered = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_HoveredR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_HoveredG", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_HoveredB", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_HoveredA", 1.0f)));
		ZeroGUI::Colors::Button_Active = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_ActiveR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_ActiveG", 0.005f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_ActiveB", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Button_ActiveA", 1.0f)));
		ZeroGUI::Colors::Checkbox_Idle = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_IdleR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_IdleG", 0.75f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_IdleB", 0.75f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_IdleA", 0.8f)));
		ZeroGUI::Colors::Checkbox_Hovered = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_HoveredR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_HoveredG", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_HoveredB", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_HoveredA", 1.0f)));
		ZeroGUI::Colors::Checkbox_Enabled = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_EnabledR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_EnabledG", 0.005f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_EnabledB", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Checkbox_EnabledA", 1.0f)));
		ZeroGUI::Colors::Combobox_Idle = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_IdleR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_IdleG", 0.75f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_IdleB", 0.75f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_IdleA", 0.8f)));
		ZeroGUI::Colors::Combobox_Hovered = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_HoveredR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_HoveredG", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_HoveredB", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_HoveredA", 1.0f)));
		ZeroGUI::Colors::Combobox_Elements = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_ElementsR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_ElementsG", 0.005f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_ElementsB", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Combobox_ElementsA", 1.0f)));
		ZeroGUI::Colors::Slider_Idle = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_IdleR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_IdleG", 0.75f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_IdleB", 0.75f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_IdleA", 0.8f)));
		ZeroGUI::Colors::Slider_Hovered = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_HoveredR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_HoveredG", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_HoveredB", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_HoveredA", 1.0f)));
		ZeroGUI::Colors::Slider_Progress = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_ProgressR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_ProgressG", 0.50f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_ProgressB", 0.75f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_ProgressA", 1.0f)));
		ZeroGUI::Colors::Slider_Button = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_ButtonR", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_ButtonG", 0.005f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_ButtonB", 1.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "Slider_ButtonA", 1.0f)));
		ZeroGUI::Colors::ColorPicker_Background = TFD_SDK::FLinearColor(
			static_cast<float>(ini.GetDoubleValue("Colors", "ColorPicker_BackgroundR", 0.0f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "ColorPicker_BackgroundG", 0.01f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "ColorPicker_BackgroundB", 0.01f)),
			static_cast<float>(ini.GetDoubleValue("Colors", "ColorPicker_BackgroundA", 0.8f)));
		menuWidth = ini.GetDoubleValue("WindowSize", "Width", 600.0f);  // Default to 600
		menuHeight = ini.GetDoubleValue("WindowSize", "Height", 800.0f); // Default to 800
	}
}

void SaveCFG()
{
	ini.SetDoubleValue("Menu", "X", cfg_WindowPos.X);
	ini.SetDoubleValue("Menu", "Y", cfg_WindowPos.Y);

	ini.SetBoolValue("ESP", "EnablePlayerEnemyESP", cfg_EnablePlayerEnemyESP);
	ini.SetBoolValue("ESP", "EnablePlayerNames", cfg_DrawPlayerNames);
	ini.SetBoolValue("ESP", "EnablePlayerBoxes", cfg_DrawPlayerBoxes);
	ini.SetBoolValue("ESP", "EnablePlayerLines", cfg_DrawPlayerLines);
	ini.SetBoolValue("ESP", "EnableEnemyNames", cfg_DrawEnemyNames);
	ini.SetBoolValue("ESP", "EnableEnemyBoxes", cfg_DrawEnemyBoxes);
	ini.SetBoolValue("ESP", "EnableEnemyLines", cfg_DrawEnemyLines);
	ini.SetBoolValue("ESP", "EnableItemESP", cfg_EnableItemESP);
	ini.SetBoolValue("ESP", "EnableItemBoxes", cfg_DrawItemBoxes);
	ini.SetBoolValue("ESP", "EnableItemNames", cfg_DrawItemNames);
	ini.SetBoolValue("ESP", "EnableItemLines", cfg_DrawItemLines);
	ini.SetBoolValue("ESP", "EnableCodedVaults", cfg_DrawVaults);
	ini.SetBoolValue("ESP", "EnableVulgusResourceCrates", cfg_DrawLootBoxes);
	ini.SetBoolValue("ESP", "EnableMunitionBarrels", cfg_DrawLootBarrels);
	ini.SetBoolValue("ESP", "DrawCirclesInstead", cfg_DrawCirclesInstead);
	ini.SetDoubleValue("ESP", "ShapeRangeScale", cfg_DistanceScale);
	ini.SetDoubleValue("ESP", "ShapeScaleFactor", cfg_ScaleFactor);

	ini.SetBoolValue("Aimbot", "EnableAimbotHold", cfg_EnableAimbotHold);
	ini.SetBoolValue("Aimbot", "EnableAimbotToggle", cfg_EnableAimbotToggle);
	ini.SetDoubleValue("Aimbot", "AimbotHoldKey", cfg_AimbotHoldKey);
	ini.SetDoubleValue("Aimbot", "AimbotToggleKey", cfg_AimbotToggleKey);
	ini.SetBoolValue("Aimbot", "EnableController", cfg_AimbotController);
	ini.SetDoubleValue("Aimbot", "AimbotFOV", cfg_AimbotFOV);
	ini.SetDoubleValue("Aimbot", "AimbotSmooth", cfg_AimbotSmoothing);

	ini.SetBoolValue("Aimbot", "EnableNoSpread", cfg_AimbotNoSpread);
	ini.SetBoolValue("Aimbot", "EnableNoRecoil", cfg_AimbotNoRecoil);
	ini.SetBoolValue("Aimbot", "EnableRapidFire", cfg_AimbotRapidFire);
	ini.SetDoubleValue("Extra", "InstantInfilKey", cfg_InstantInfilKey);
	ini.SetBoolValue("Extra", "EnableNoReload", cfg_NoReload);
	ini.SetBoolValue("Extra", "EnableVacuum", cfg_LootVacuum);
	ini.SetDoubleValue("Extra", "VacuumRange", cfg_LootVacuumRange);
	ini.SetDoubleValue("Extra", "VacuumKey", cfg_LootVacuumKey);

	ini.SetBoolValue("Extra", "CacheNames", cfg_CacheEnemyNames);
	ini.SetBoolValue("Extra", "CacheBones", cfg_CacheEnemyBones);
	ini.SetDoubleValue("Extra", "HotSwapKey", cfg_HotSwapKey);
	ini.SetBoolValue("Extra", "HotSwapOverlay", cfg_HotSwapOverlay);

	ini.SetDoubleValue("Extra", "HotSwapSlot1", HotSwapCharacters[0]);
	ini.SetDoubleValue("Extra", "HotSwapSlot2", HotSwapCharacters[1]);
	ini.SetDoubleValue("Extra", "HotSwapSlot3", HotSwapCharacters[2]);
	ini.SetDoubleValue("Extra", "HotSwapSlot4", HotSwapCharacters[3]);

	ini.SetDoubleValue("Extra", "Timescale", cfg_TimeScale);
	ini.SetDoubleValue("Extra", "TimescaleKey", cfg_TimeScaleKey);
	ini.SetDoubleValue("Extra", "TimescaleHoldKey", cfg_TimeScaleHoldKey);

	ini.SetDoubleValue("Colors", "HealthColorR", HealthColor.R);
	ini.SetDoubleValue("Colors", "HealthColorG", HealthColor.G);
	ini.SetDoubleValue("Colors", "HealthColorB", HealthColor.B);
	ini.SetDoubleValue("Colors", "HealthColorA", HealthColor.A);

	ini.SetDoubleValue("Colors", "ManaColorR", ManaColor.R);
	ini.SetDoubleValue("Colors", "ManaColorG", ManaColor.G);
	ini.SetDoubleValue("Colors", "ManaColorB", ManaColor.B);
	ini.SetDoubleValue("Colors", "ManaColorA", ManaColor.A);

	ini.SetDoubleValue("Colors", "GoldColorR", GoldColor.R);
	ini.SetDoubleValue("Colors", "GoldColorG", GoldColor.G);
	ini.SetDoubleValue("Colors", "GoldColorB", GoldColor.B);
	ini.SetDoubleValue("Colors", "GoldColorA", GoldColor.A);

	ini.SetDoubleValue("Colors", "KuiperColorR", KuiperColor.R);
	ini.SetDoubleValue("Colors", "KuiperColorG", KuiperColor.G);
	ini.SetDoubleValue("Colors", "KuiperColorB", KuiperColor.B);
	ini.SetDoubleValue("Colors", "KuiperColorA", KuiperColor.A);

	ini.SetDoubleValue("Colors", "Tier1ColorR", Tier1Color.R);
	ini.SetDoubleValue("Colors", "Tier1ColorG", Tier1Color.G);
	ini.SetDoubleValue("Colors", "Tier1ColorB", Tier1Color.B);
	ini.SetDoubleValue("Colors", "Tier1ColorA", Tier1Color.A);

	ini.SetDoubleValue("Colors", "Tier2ColorR", Tier2Color.R);
	ini.SetDoubleValue("Colors", "Tier2ColorG", Tier2Color.G);
	ini.SetDoubleValue("Colors", "Tier2ColorB", Tier2Color.B);
	ini.SetDoubleValue("Colors", "Tier2ColorA", Tier2Color.A);

	ini.SetDoubleValue("Colors", "Tier3ColorR", Tier3Color.R);
	ini.SetDoubleValue("Colors", "Tier3ColorG", Tier3Color.G);
	ini.SetDoubleValue("Colors", "Tier3ColorB", Tier3Color.B);
	ini.SetDoubleValue("Colors", "Tier3ColorA", Tier3Color.A);

	ini.SetDoubleValue("Colors", "Tier4ColorR", Tier4Color.R);
	ini.SetDoubleValue("Colors", "Tier4ColorG", Tier4Color.G);
	ini.SetDoubleValue("Colors", "Tier4ColorB", Tier4Color.B);
	ini.SetDoubleValue("Colors", "Tier4ColorA", Tier4Color.A);

	ini.SetDoubleValue("Colors", "CursorColorR", ZeroGUI::Colors::CursorColor.R);
	ini.SetDoubleValue("Colors", "CursorColorG", ZeroGUI::Colors::CursorColor.G);
	ini.SetDoubleValue("Colors", "CursorColorB", ZeroGUI::Colors::CursorColor.B);
	ini.SetDoubleValue("Colors", "CursorColorA", ZeroGUI::Colors::CursorColor.A);

	ini.SetDoubleValue("Colors", "TextR", ZeroGUI::Colors::Text.R);
	ini.SetDoubleValue("Colors", "TextG", ZeroGUI::Colors::Text.G);
	ini.SetDoubleValue("Colors", "TextB", ZeroGUI::Colors::Text.B);
	ini.SetDoubleValue("Colors", "TextA", ZeroGUI::Colors::Text.A);

	ini.SetDoubleValue("Colors", "Window_HeaderR", ZeroGUI::Colors::Window_Header.R);
	ini.SetDoubleValue("Colors", "Window_HeaderG", ZeroGUI::Colors::Window_Header.G);
	ini.SetDoubleValue("Colors", "Window_HeaderB", ZeroGUI::Colors::Window_Header.B);
	ini.SetDoubleValue("Colors", "Window_HeaderA", ZeroGUI::Colors::Window_Header.A);

	ini.SetDoubleValue("Colors", "Text_ShadowR", ZeroGUI::Colors::Text_Shadow.R);
	ini.SetDoubleValue("Colors", "Text_ShadowG", ZeroGUI::Colors::Text_Shadow.G);
	ini.SetDoubleValue("Colors", "Text_ShadowB", ZeroGUI::Colors::Text_Shadow.B);
	ini.SetDoubleValue("Colors", "Text_ShadowA", ZeroGUI::Colors::Text_Shadow.A);

	ini.SetDoubleValue("Colors", "Window_BackgroundR", ZeroGUI::Colors::Window_Background.R);
	ini.SetDoubleValue("Colors", "Window_BackgroundG", ZeroGUI::Colors::Window_Background.G);
	ini.SetDoubleValue("Colors", "Window_BackgroundB", ZeroGUI::Colors::Window_Background.B);
	ini.SetDoubleValue("Colors", "Window_BackgroundA", ZeroGUI::Colors::Window_Background.A);

	ini.SetDoubleValue("Colors", "Text_OutlineR", ZeroGUI::Colors::Text_Outline.R);
	ini.SetDoubleValue("Colors", "Text_OutlineG", ZeroGUI::Colors::Text_Outline.G);
	ini.SetDoubleValue("Colors", "Text_OutlineB", ZeroGUI::Colors::Text_Outline.B);
	ini.SetDoubleValue("Colors", "Text_OutlineA", ZeroGUI::Colors::Text_Outline.A);

	ini.SetDoubleValue("Colors", "Slider_ButtonR", ZeroGUI::Colors::Slider_Button.R);
	ini.SetDoubleValue("Colors", "Slider_ButtonG", ZeroGUI::Colors::Slider_Button.G);
	ini.SetDoubleValue("Colors", "Slider_ButtonB", ZeroGUI::Colors::Slider_Button.B);
	ini.SetDoubleValue("Colors", "Slider_ButtonA", ZeroGUI::Colors::Slider_Button.A);

	ini.SetDoubleValue("Colors", "Button_IdleR", ZeroGUI::Colors::Button_Idle.R);
	ini.SetDoubleValue("Colors", "Button_IdleG", ZeroGUI::Colors::Button_Idle.G);
	ini.SetDoubleValue("Colors", "Button_IdleB", ZeroGUI::Colors::Button_Idle.B);
	ini.SetDoubleValue("Colors", "Button_IdleA", ZeroGUI::Colors::Button_Idle.A);

	ini.SetDoubleValue("Colors", "Slider_IdleR", ZeroGUI::Colors::Slider_Idle.R);
	ini.SetDoubleValue("Colors", "Slider_IdleG", ZeroGUI::Colors::Slider_Idle.G);
	ini.SetDoubleValue("Colors", "Slider_IdleB", ZeroGUI::Colors::Slider_Idle.B);
	ini.SetDoubleValue("Colors", "Slider_IdleA", ZeroGUI::Colors::Slider_Idle.A);

	ini.SetDoubleValue("Colors", "Button_HoveredR", ZeroGUI::Colors::Button_Hovered.R);
	ini.SetDoubleValue("Colors", "Button_HoveredG", ZeroGUI::Colors::Button_Hovered.G);
	ini.SetDoubleValue("Colors", "Button_HoveredB", ZeroGUI::Colors::Button_Hovered.B);
	ini.SetDoubleValue("Colors", "Button_HoveredA", ZeroGUI::Colors::Button_Hovered.A);

	ini.SetDoubleValue("Colors", "Slider_HoveredR", ZeroGUI::Colors::Slider_Hovered.R);
	ini.SetDoubleValue("Colors", "Slider_HoveredG", ZeroGUI::Colors::Slider_Hovered.G);
	ini.SetDoubleValue("Colors", "Slider_HoveredB", ZeroGUI::Colors::Slider_Hovered.B);
	ini.SetDoubleValue("Colors", "Slider_HoveredA", ZeroGUI::Colors::Slider_Hovered.A);

	ini.SetDoubleValue("Colors", "Button_ActiveR", ZeroGUI::Colors::Button_Active.R);
	ini.SetDoubleValue("Colors", "Button_ActiveG", ZeroGUI::Colors::Button_Active.G);
	ini.SetDoubleValue("Colors", "Button_ActiveB", ZeroGUI::Colors::Button_Active.B);
	ini.SetDoubleValue("Colors", "Button_ActiveA", ZeroGUI::Colors::Button_Active.A);

	ini.SetDoubleValue("Colors", "Slider_ProgressR", ZeroGUI::Colors::Slider_Progress.R);
	ini.SetDoubleValue("Colors", "Slider_ProgressG", ZeroGUI::Colors::Slider_Progress.G);
	ini.SetDoubleValue("Colors", "Slider_ProgressB", ZeroGUI::Colors::Slider_Progress.B);
	ini.SetDoubleValue("Colors", "Slider_ProgressA", ZeroGUI::Colors::Slider_Progress.A);

	ini.SetDoubleValue("Colors", "Checkbox_IdleR", ZeroGUI::Colors::Checkbox_Idle.R);
	ini.SetDoubleValue("Colors", "Checkbox_IdleG", ZeroGUI::Colors::Checkbox_Idle.G);
	ini.SetDoubleValue("Colors", "Checkbox_IdleB", ZeroGUI::Colors::Checkbox_Idle.B);
	ini.SetDoubleValue("Colors", "Checkbox_IdleA", ZeroGUI::Colors::Checkbox_Idle.A);

	ini.SetDoubleValue("Colors", "Checkbox_HoveredR", ZeroGUI::Colors::Checkbox_Hovered.R);
	ini.SetDoubleValue("Colors", "Checkbox_HoveredG", ZeroGUI::Colors::Checkbox_Hovered.G);
	ini.SetDoubleValue("Colors", "Checkbox_HoveredB", ZeroGUI::Colors::Checkbox_Hovered.B);
	ini.SetDoubleValue("Colors", "Checkbox_HoveredA", ZeroGUI::Colors::Checkbox_Hovered.A);

	ini.SetDoubleValue("Colors", "Checkbox_EnabledR", ZeroGUI::Colors::Checkbox_Enabled.R);
	ini.SetDoubleValue("Colors", "Checkbox_EnabledG", ZeroGUI::Colors::Checkbox_Enabled.G);
	ini.SetDoubleValue("Colors", "Checkbox_EnabledB", ZeroGUI::Colors::Checkbox_Enabled.B);
	ini.SetDoubleValue("Colors", "Checkbox_EnabledA", ZeroGUI::Colors::Checkbox_Enabled.A);

	ini.SetDoubleValue("Colors", "ColorPicker_BackgroundR", ZeroGUI::Colors::ColorPicker_Background.R);
	ini.SetDoubleValue("Colors", "ColorPicker_BackgroundG", ZeroGUI::Colors::ColorPicker_Background.G);
	ini.SetDoubleValue("Colors", "ColorPicker_BackgroundB", ZeroGUI::Colors::ColorPicker_Background.B);
	ini.SetDoubleValue("Colors", "ColorPicker_BackgroundA", ZeroGUI::Colors::ColorPicker_Background.A);

	ini.SetDoubleValue("WindowSize", "Width", menuWidth);
	ini.SetDoubleValue("WindowSize", "Height", menuHeight);

	ini.SaveFile("cfg.ini");
}


void WriteEnemyNamesData(){
	std::ofstream outFile("NameCache.dat", std::ios::binary);

	if (!outFile) {
		return;
	}
	outFile.clear();
	size_t size = IDNameMap.size();
	outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// Write each key-value pair
	for (const auto& pair : IDNameMap) {
		// Write the key
		outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));

		// Write the length of the string value
		size_t strLength = pair.second.size();
		outFile.write(reinterpret_cast<const char*>(&strLength), sizeof(strLength));

		// Write the string value
		outFile.write(pair.second.data(), strLength);
	}

	outFile.close();
}

std::unordered_map<int, std::string> ReadEnemyNamesData(){
	std::unordered_map<int, std::string> map;
	std::ifstream inFile("NameCache.dat", std::ios::binary);

	if (!inFile){
		return map;
	}

	// Read the size of the map
	size_t size = 0;
	inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

	for (size_t i = 0; i < size; ++i) {
		int key;
		size_t strLength;
		std::string value;

		// Read the key
		inFile.read(reinterpret_cast<char*>(&key), sizeof(key));

		// Read the length of the string value
		inFile.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));

		// Read the string value
		value.resize(strLength);
		inFile.read(&value[0], strLength);

		// Insert into the map
		map[key] = value;
	}

	inFile.close();
	return map;
}

void WriteEnemyBonesData() {
	std::ofstream outFile("BoneCache.dat", std::ios::binary);

	if (!outFile) {
		return;
	}
	outFile.clear();
	// Write the size of the map first
	size_t mapSize = IDBoneMap.size();
	outFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

	// Write each key-vector pair
	for (const auto& pair : IDBoneMap) {
		// Write the key
		outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));

		// Write the size of the vector
		size_t vectorSize = pair.second.size();
		outFile.write(reinterpret_cast<const char*>(&vectorSize), sizeof(vectorSize));

		// Write the vector elements
		outFile.write(reinterpret_cast<const char*>(pair.second.data()), vectorSize * sizeof(int));
	}

	outFile.close();
}

std::unordered_map<int, std::vector<int>> ReadEnemyBonesData() {
	std::unordered_map<int, std::vector<int>> map;
	std::ifstream inFile("BoneCache.dat", std::ios::binary);

	if (!inFile) {
		return map;
	}

	// Read the size of the map
	size_t mapSize = 0;
	inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

	for (size_t i = 0; i < mapSize; ++i) {
		int key;
		size_t vectorSize;
		std::vector<int> value;

		// Read the key
		inFile.read(reinterpret_cast<char*>(&key), sizeof(key));

		// Read the size of the vector
		inFile.read(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));

		// Resize the vector to hold the elements
		value.resize(vectorSize);

		// Read the vector elements
		inFile.read(reinterpret_cast<char*>(value.data()), vectorSize * sizeof(int));

		// Insert into the map
		map[key] = value;
	}

	inFile.close();
	return map;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, hModule, 0, 0);

		return true;
	}
	return false;
}

DWORD WINAPI Init(HMODULE Module)
{
#ifdef IS_DEBUG

	AllocConsole();
	FILE* Dummy;
	freopen_s(&Dummy, "CONOUT$", "w", stdout);
	freopen_s(&Dummy, "CONIN$", "r", stdin);
	std::cout << "DescentInternal - Started\n";

#endif // IS_DEBUG
	int procID = GetCurrentProcessId();
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	sigmod GameModule = { NULL,NULL };
	do
	{
		if (!strcmp(mEntry.szModule, "M1-Win64-Shipping.exe"))
		{
			GameModule = { (uintptr_t)mEntry.hModule, mEntry.modBaseSize };
		}
	} while (Module32Next(hModule, &mEntry));
	CloseHandle(hModule);
	Sleep(1000);
	if (GameModule.dwBase)
	{
#ifdef IS_DEBUG
		std::cout << "DescentInternal - Found Module\n";
#endif // IS_DEBUG
		uintptr_t GNamePtr = FindSignature(procID, GameModule, GNamesSig, GNamesMask);
		if (!GNamePtr)
		{
			throw std::runtime_error("Unable to find GNames.");
			return 1;
		}
		GNamePtr = GameModule.dwBase + GNamePtr;
		uintptr_t GNameOffsetAddress = GNamePtr + 3;
		int32_t GNameOffsetRelative = *reinterpret_cast<int32_t*>(GNameOffsetAddress);
		uintptr_t GNameAddress = (GNamePtr + 7) + GNameOffsetRelative;
		uintptr_t GNameOffset = GNameAddress - GameModule.dwBase;
		TFD_SDK::Offsets::GNames = GNameOffset;
#ifdef IS_DEBUG
		std::cout << "DescentInternal - Found GNames at: " << std::hex << GNameOffset << "\n";
		Sleep(1000);
#endif // IS_DEBUG

		/*uintptr_t GObjsPtr = FindSignature(procID, GameModule, GObjectsSig, GObjectsMask);
		if (!GObjsPtr)
		{
			throw std::runtime_error("Unable to find GObjects.");
			return 1;
		}*/
		//GObjsPtr = GameModule.dwBase + GObjsPtr;
		//uintptr_t GObjOffsetAddress = GObjsPtr + 3;
		//int32_t GObjOffsetRelative = *reinterpret_cast<int32_t*>(GObjOffsetAddress);
		//uintptr_t GObjAddress = (GObjsPtr + 7) + GObjOffsetRelative;
		//uintptr_t GObjeOffset = GObjAddress - GameModule.dwBase;
		TFD_SDK::Offsets::GObjects = 0x09CC43E0;
#ifdef IS_DEBUG
		std::cout << "DescentInternal - Found GObjects at: " << std::hex << GObjsPtr << std::dec << "\n";
		Sleep(1000);
#endif // IS_DEBUG
		//TFD_SDK::Offsets::GObjects = 0x9B29CF0;

		uintptr_t SpreadPtr = FindSignature(procID, GameModule, NoSpreadSig, NoSpreadMask);
		if (!SpreadPtr)
		{
			throw std::runtime_error("Unable to find NoSpread.");
			return 1;
		}
		SpreadPtr = GameModule.dwBase + SpreadPtr;
		NoSpreadAddress = (reinterpret_cast<uint8_t*>(SpreadPtr) - 0x9);// +0x16);
#ifdef IS_DEBUG
		std::cout << "DescentInternal - Found NoSpread at " << std::hex << SpreadPtr << std::dec << "\n";
		Sleep(1000);
#endif // IS_DEBUG

		uintptr_t RecoilPtr = FindSignature(procID, GameModule, NoRecoilSig, NoRecoilMask);
		if (!RecoilPtr)
		{
			throw std::runtime_error("Unable to find NoRecoil.");
			return 1;
		}
		RecoilPtr = GameModule.dwBase + RecoilPtr;
		NoRecoilAddress = reinterpret_cast<uint8_t*>(RecoilPtr + 2);
#ifdef IS_DEBUG
		std::cout << "DescentInternal - Found NoRecoil at " << std::hex << RecoilPtr << std::dec << "\n";
		Sleep(1000);
#endif // IS_DEBUG

		uintptr_t RapidFirePtr = FindSignature(procID, GameModule, RapidFireSig, RapidFireMask);
		if (!RapidFirePtr)
		{
			throw std::runtime_error("Unable to find Rapidfire.");
			return 1;
		}
		RapidFirePtr = GameModule.dwBase + RapidFirePtr;
		RapidFireAddress = reinterpret_cast<uint8_t*>(RapidFirePtr);
#ifdef IS_DEBUG
		std::cout << "DescentInternal - Found RapidFire at " << std::hex << RapidFirePtr << std::dec << "\n";
		Sleep(1000);
#endif // IS_DEBUG

		//NoSpreadOriginal = *NoSpreadAddress;
		// This code was the original no-spread by copying and setting the ModR/M bits
		/*
		000 = XMM0
		001 = XMM1
		010 = XMM2
		011 = XMM3
		100 = XMM4
		101 = XMM5
		110 = XMM6
		111 = XMM7
		*/
		/*std::bitset<8> modifiedByte(NoSpreadOriginal);
		if (modifiedByte[5])
		{
			modifiedByte.set(2);
		}
		else
		{
			modifiedByte.reset(2);
		}
		if (modifiedByte[4])
		{
			modifiedByte.set(1);
		}
		else
		{
			modifiedByte.reset(1);
		}
		if (modifiedByte[3])
		{
			modifiedByte.set(0);
		}
		else
		{
			modifiedByte.reset(0);
		}

		NoSpreadCheat = (uint8_t)modifiedByte.to_ulong();*/
	}
	else
	{
		throw std::runtime_error("Unable to get game module.");
		return 1;
	}

	TFD_SDK::FName::InitInternal();
	TFD_SDK::UObject::GObjects.InitGObjects();

	bool isPostRenderHooked = false;
	do
	{
		TFD_SDK::UWorld* world = TFD_SDK::UWorld::GetWorld();
		if (world && world->IsA(TFD_SDK::UWorld::StaticClass()))
		{
			if (!GWorld)
			{
				GWorld = world;
				uintptr_t GWorldOffset = (uintptr_t)world - GameModule.dwBase;
				TFD_SDK::Offsets::GWorld = GWorldOffset;
			}
			TFD_SDK::ULocalPlayer* LocalPl = (TFD_SDK::ULocalPlayer*)((TFD_SDK::UGameplayStatics*)TFD_SDK::UGameplayStatics::StaticClass())->GetPlayerController(GWorld, 0)->Player;
			if (LocalPl)
			{
				if (!LocalPlayer)
				{
					LocalPlayer = LocalPl;
				}
				TFD_SDK::UGameViewportClient* ViewportClient = LocalPlayer->ViewportClient;
				if (ViewportClient)
				{
					if (!LocalView)
					{
						LocalView = ViewportClient;
					}
					ULONG64* Func = GetDrawTransitionVTableAddress(LocalView);
					if (Func)
					{
						M1org = (decltype(M1org))*Func;
						__int64 ptr = (__int64)YourHookProc;
						DWORD old;
						VirtualProtect(Func, sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
						memcpy(Func, &ptr, 8);
						VirtualProtect(Func, sizeof(void*), old, NULL);
						isPostRenderHooked = true;
					}
				}
			}
		}
		else
		{
			Sleep(1000);
		}
	} while (!isPostRenderHooked);
#ifdef IS_DEBUG
	std::cout << "DescentInternal - PostRender Hooked\n";
#endif // IS_DEBUG
	ini.SetUnicode();
	LoadCFG();
	return 1;
}

ULONG64* GetDrawTransitionVTableAddress(TFD_SDK::UGameViewportClient* vp)
{
	if (vp)
	{
		__int64** vtable = NULL;
		int Index = 0;
		vtable = *reinterpret_cast<__int64***>(vp);
		if (vtable)
		{
			for (int i = 0; i < 0x200; i++)
			{
				BYTE* code = (BYTE*)vtable[i];
				for (int j = 0; j < 0x400; j++)
				{
					BYTE* p = code + j;
					if (
						(p[0] == 0x4C && p[1] == 0x8D && p[2] == 0x05) ||
						(p[0] == 0x48 && p[1] == 0x8D && p[2] == 0x15)
						)
					{
						int offset = *(int*)(p + 3);
						wchar_t* str = (wchar_t*)(p + 7 + offset);
						if (str == std::wstring(L"PAUSED"))
						{
							Index = i;
							goto ext;
						}
					}
				}
			}
		}
	ext:
		if (Index)
		{
			return (ULONG64*)&vtable[Index];
		}
	}
	return NULL;
}

bool MemoryCompare(const BYTE* bData, const BYTE* bSig, const char* szMask)
{
	for (; *szMask; szMask++, bData++, bSig++)
		if (*szMask == 'x' && *bData != *bSig)
			return false;
	return true;
}

// This returns the OFFSET so you need to add the OFFSET + BASE for the desired address!!!
uintptr_t FindSignature(int procID, sigmod mod, const char* sig, const char* mask)
{
	BYTE* data = new BYTE[mod.dwSize];
	SIZE_T bytesRead;

	Toolhelp32ReadProcessMemory(procID, (LPVOID)(mod.dwBase), data, mod.dwSize, &bytesRead);

	for (uintptr_t i = 0; i < mod.dwSize; i++) {
		if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
			delete[] data;
			return i;
		}
	}
	delete[] data;
	return NULL;
}
