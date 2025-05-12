// Copyright EPortfolio


#include "ELogHelpers.h"
#include "GameFramework/Actor.h"

void ELogHelpers::PrintNet(AActor* InActor, FString PrintStr)
{
	if (GEngine)
	{
		FString str;

		switch (InActor->GetNetMode())
		{
		case ENetMode::NM_Standalone     : str.Append(FString(TEXT("NM_Standalone::")));      break;
		case ENetMode::NM_DedicatedServer: str.Append(FString(TEXT("NM_DedicatedServer::"))); break;
		case ENetMode::NM_ListenServer   : str.Append(FString(TEXT("NM_ListenServer::")));    break;
		case ENetMode::NM_Client         : str.Append(FString(TEXT("NM_Client::")));          break;
		}

		switch (InActor->GetLocalRole())
		{
		case ENetRole::ROLE_Authority      : str.Append(FString(TEXT("ROLE_Authority::")));      break;
		case ENetRole::ROLE_AutonomousProxy: str.Append(FString(TEXT("ROLE_AutonomousProxy::"))); break;
		case ENetRole::ROLE_SimulatedProxy : str.Append(FString(TEXT("ROLE_SimulatedProxy::")));    break;
		case ENetRole::ROLE_None           : str.Append(FString(TEXT("ROLE_None::")));          break;
		}


		switch (InActor->GetRemoteRole())
		{
		case ENetRole::ROLE_Authority      : str.Append(FString(TEXT("ROLE_Authority::")));      break;
		case ENetRole::ROLE_AutonomousProxy: str.Append(FString(TEXT("ROLE_AutonomousProxy::")));break;
		case ENetRole::ROLE_SimulatedProxy : str.Append(FString(TEXT("ROLE_SimulatedProxy::"))); break;
		case ENetRole::ROLE_None           : str.Append(FString(TEXT("ROLE_None::")));           break;
		}

		str.Append(PrintStr);

		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Emerald, str);
	}
}

void ELogHelpers::PrintIsValid(UObject* InObj, const WIDECHAR* InName)
{
	FString str;

	str.Append(FString(InName));

	if (InObj)
	{
		str.Append(FString("is Valid"));
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, str);
	}
	else
	{
		str.Append(FString("is Not Valid"));
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, str);
	}
}

void ELogHelpers::PrintFunctionCall()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("%s Call"), __FUNCTION__));
}
