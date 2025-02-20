#include "SDK/Basic.hpp"
#include "TFD_SDK.h"
//#include "SDK/M1_parameters.hpp"


namespace TFD_SDK
{
	class UEngine* UEngine::GetEngine()
	{
		static UEngine* GEngine = nullptr;

		if (GEngine)
			return GEngine;

		/* (Re-)Initialize if GEngine is nullptr */
		for (int i = 0; i < UObject::GObjects->Num(); i++)
		{
			UObject* Obj = UObject::GObjects->GetByIndex(i);

			if (!Obj)
				continue;

			if (Obj->IsA(UEngine::StaticClass()) && !Obj->IsDefaultObject())
			{
				GEngine = static_cast<UEngine*>(Obj);
				break;
			}
		}

		return GEngine;
	}

	class UWorld* UWorld::GetWorld()
	{
		static UWorld* World = nullptr;
		static int WorldIndex = -1;

		if (World == nullptr)
		{
			for (int i = 0; i < UObject::GObjects->Num(); i++)
			{
				UObject* Obj = UObject::GObjects->GetByIndex(i);

				if (!Obj)
					continue;

				if (Obj->Flags & EObjectFlags::LoadCompleted && Obj->IsA(UWorld::StaticClass()) && !Obj->IsDefaultObject())
				{

					WorldIndex = i;
					World = static_cast<UWorld*>(Obj);
					//std::cout << "DescentInternal - World Found: " << std::hex << World << std::dec << " with Index: " << WorldIndex << "\n";
					break;
				}
			}
			return World;
		}
		else
		{
			UObject* Obj = UObject::GObjects->GetByIndex(WorldIndex);

			if (!Obj)
			{
				//std::cout << "DescentInternal - World Obj Invalid: " << std::hex << World << std::dec << "\n";
				World = nullptr;
				WorldIndex = -1;
				return World;
			}
			else
			{
				if (!(Obj->Flags & TFD_SDK::EObjectFlags::LoadCompleted) || !Obj->IsA(UWorld::StaticClass()) || Obj->IsDefaultObject())
				{
					//std::cout << "DescentInternal - World Obj Not World or is DefaultObject: " << std::hex << World << std::dec << "\n";
					World = nullptr;
					WorldIndex = -1;
					return World;
				}
			}
			return World;
		}
	}


	void UM1PrivateOnlineServiceComponent::ServerChangePlayer(const struct FM1TemplateId& InCharacterTid)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1PrivateOnlineServiceComponent", "ServerChangePlayer");

		M1PrivateOnlineServiceComponent_ServerChangePlayer Parms{};

		Parms.InCharacterTid = std::move(InCharacterTid);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1WeaponRoundsComponent::ClientFillCurrentRoundByServer()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1WeaponRoundsComponent", "ClientFillCurrentRoundByServer");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	bool AM1Character::IsDead() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1Character", "IsDead");

		M1Character_IsDead Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	void AM1Player::RequestTeleportAtSequence(const struct FVector& InLocation, const struct FRotator& InRotation)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1Player", "RequestTeleportAtSequence");

		M1Player_RequestTeleportAtSequence Parms{};

		Parms.InLocation = std::move(InLocation);
		Parms.InRotation = std::move(InRotation);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	bool AActor::K2_SetActorLocation(const struct FVector& NewLocation, bool bSweep, struct FHitResult* SweepHitResult, bool bTeleport)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Actor", "K2_SetActorLocation");

		Actor_K2_SetActorLocation Parms{};

		Parms.NewLocation = std::move(NewLocation);
		Parms.bSweep = bSweep;
		Parms.bTeleport = bTeleport;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		if (SweepHitResult != nullptr)
			*SweepHitResult = std::move(Parms.SweepHitResult);

		return Parms.ReturnValue;
	}

	float AActor::GetDistanceTo(const class AActor* OtherActor) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Actor", "GetDistanceTo");

		Actor_GetDistanceTo Parms{};

		Parms.OtherActor = OtherActor;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FVector AActor::K2_GetActorLocation() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Actor", "K2_GetActorLocation");

		Actor_K2_GetActorLocation Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	void AController::SetControlRotation(const struct FRotator& NewRotation)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Controller", "SetControlRotation");

		Controller_SetControlRotation Parms{};

		Parms.NewRotation = std::move(NewRotation);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	bool AController::LineOfSightTo(const class AActor* Other, const struct FVector& ViewPoint, bool bAlternateChecks) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Controller", "LineOfSightTo");

		Controller_LineOfSightTo Parms{};

		Parms.Other = Other;
		Parms.ViewPoint = std::move(ViewPoint);
		Parms.bAlternateChecks = bAlternateChecks;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FVector APlayerCameraManager::GetCameraLocation() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("PlayerCameraManager", "GetCameraLocation");

		PlayerCameraManager_GetCameraLocation Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FRotator APlayerCameraManager::GetCameraRotation() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("PlayerCameraManager", "GetCameraRotation");

		PlayerCameraManager_GetCameraRotation Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	bool APlayerController::GetMousePosition(float* LocationX, float* LocationY) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("PlayerController", "GetMousePosition");

		PlayerController_GetMousePosition Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		if (LocationX != nullptr)
			*LocationX = Parms.LocationX;

		if (LocationY != nullptr)
			*LocationY = Parms.LocationY;

		return Parms.ReturnValue;
	}

	void UActorComponent::Deactivate()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("ActorComponent", "Deactivate");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	bool UActorComponent::IsActive() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("ActorComponent", "IsActive");

		ActorComponent_IsActive Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FTransform USceneComponent::K2_GetComponentToWorld() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("SceneComponent", "K2_GetComponentToWorld");

		SceneComponent_K2_GetComponentToWorld Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	class FName USkinnedMeshComponent::GetBoneName(int32 BoneIndex) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("SkinnedMeshComponent", "GetBoneName");

		SkinnedMeshComponent_GetBoneName Parms{};

		Parms.BoneIndex = BoneIndex;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	class APlayerController* UGameplayStatics::GetPlayerController(const class UObject* WorldContextObject, int32 PlayerIndex)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("GameplayStatics", "GetPlayerController");

		GameplayStatics_GetPlayerController Parms{};

		Parms.WorldContextObject = WorldContextObject;
		Parms.PlayerIndex = PlayerIndex;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	bool UGameplayStatics::ProjectWorldToScreen(class APlayerController* Player, const struct FVector& WorldPosition, struct FVector2D* ScreenPosition, bool bPlayerViewportRelative)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("GameplayStatics", "ProjectWorldToScreen");

		GameplayStatics_ProjectWorldToScreen Parms{};

		Parms.Player = Player;
		Parms.WorldPosition = std::move(WorldPosition);
		Parms.bPlayerViewportRelative = bPlayerViewportRelative;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		if (ScreenPosition != nullptr)
			*ScreenPosition = std::move(Parms.ScreenPosition);

		return Parms.ReturnValue;
	}

	double UGameplayStatics::GetWorldDeltaSeconds(const class UObject* WorldContextObject)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("GameplayStatics", "GetWorldDeltaSeconds");

		GameplayStatics_GetWorldDeltaSeconds Parms{};

		Parms.WorldContextObject = WorldContextObject;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FTransform UKismetMathLibrary::Conv_MatrixToTransform(const struct FMatrix& InMatrix)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "Conv_MatrixToTransform");

		KismetMathLibrary_Conv_MatrixToTransform Parms{};

		Parms.InMatrix = std::move(InMatrix);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FMatrix UKismetMathLibrary::Conv_TransformToMatrix(const struct FTransform& Transform)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "Conv_TransformToMatrix");

		KismetMathLibrary_Conv_TransformToMatrix Parms{};

		Parms.Transform = std::move(Transform);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FRotator UKismetMathLibrary::FindLookAtRotation(const struct FVector& Start, const struct FVector& Target)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "FindLookAtRotation");

		KismetMathLibrary_FindLookAtRotation Parms{};

		Parms.Start = std::move(Start);
		Parms.Target = std::move(Target);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FMatrix UKismetMathLibrary::Multiply_MatrixMatrix(const struct FMatrix& A, const struct FMatrix& B)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "Multiply_MatrixMatrix");

		KismetMathLibrary_Multiply_MatrixMatrix Parms{};

		Parms.A = std::move(A);
		Parms.B = std::move(B);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FRotator UKismetMathLibrary::RInterpTo(const struct FRotator& Current, const struct FRotator& Target, float DeltaTime, float InterpSpeed)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "RInterpTo");

		KismetMathLibrary_RInterpTo Parms{};

		Parms.Current = std::move(Current);
		Parms.Target = std::move(Target);
		Parms.DeltaTime = DeltaTime;
		Parms.InterpSpeed = InterpSpeed;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	double UKismetMathLibrary::Distance2D(const struct FVector2D& V1, const struct FVector2D& V2)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "Distance2D");

		KismetMathLibrary_Distance2D Parms{};

		Parms.V1 = std::move(V1);
		Parms.V2 = std::move(V2);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	void UCanvas::K2_DrawLine(const struct FVector2D& ScreenPositionA, const struct FVector2D& ScreenPositionB, float Thickness, const struct FLinearColor& RenderColor)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Canvas", "K2_DrawLine");

		Canvas_K2_DrawLine Parms{};

		Parms.ScreenPositionA = std::move(ScreenPositionA);
		Parms.ScreenPositionB = std::move(ScreenPositionB);
		Parms.Thickness = Thickness;
		Parms.RenderColor = std::move(RenderColor);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UCanvas::K2_DrawText(class UFont* RenderFont, const class FString& RenderText, const struct FVector2D& ScreenPosition, const struct FVector2D& Scale, const struct FLinearColor& RenderColor, float Kerning, const struct FLinearColor& ShadowColor, const struct FVector2D& ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, const struct FLinearColor& OutlineColor)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Canvas", "K2_DrawText");

		Canvas_K2_DrawText Parms{};

		Parms.RenderFont = RenderFont;
		Parms.RenderText = std::move(RenderText);
		Parms.ScreenPosition = std::move(ScreenPosition);
		Parms.Scale = std::move(Scale);
		Parms.RenderColor = std::move(RenderColor);
		Parms.Kerning = Kerning;
		Parms.ShadowColor = std::move(ShadowColor);
		Parms.ShadowOffset = std::move(ShadowOffset);
		Parms.bCentreX = bCentreX;
		Parms.bCentreY = bCentreY;
		Parms.bOutlined = bOutlined;
		Parms.OutlineColor = std::move(OutlineColor);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	bool AM1DropContainer::IsObtained() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1DropContainer", "IsObtained");

		M1DropContainer_IsObtained Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	int32 USkinnedMeshComponent::GetNumBones() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("SkinnedMeshComponent", "GetNumBones");

		SkinnedMeshComponent_GetNumBones Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}
	void UM1MissionTaskServiceInteraction::ServerRequestMissionTargetBeginInteraction(class AM1MissionTargetInteraction* InActor, class AM1PlayerControllerInGame* InAcceptor)
	{
		static class UFunction* Func = nullptr;
		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionTaskServiceInteraction", "ServerRequestMissionTargetBeginInteraction");
		TFD_SDK::M1MissionTaskServiceInteraction_ServerRequestMissionTargetBeginInteraction Parms{};
		Parms.InActor = InActor;
		Parms.InAcceptor = InAcceptor;
		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;
		UObject::ProcessEvent(Func, &Parms);
		Func->FunctionFlags = Flgs;
	}
}
