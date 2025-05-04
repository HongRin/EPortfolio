// Copyright EPortfolio


#include "Notify/FinishedReloadNotify.h"
#include "Component/ECombatComponent.h"

FString UFinishedReloadNotify::GetNotifyName_Implementation() const
{
    return FString("ReloadFinished");
}

void UFinishedReloadNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (MeshComp)
    {
        UECombatComponent* CombatComponent = (MeshComp->GetOwner())->GetComponentByClass<UECombatComponent>();
        
        if (CombatComponent)
        {
            CombatComponent->FinsishedReload();
        }
    }
}
