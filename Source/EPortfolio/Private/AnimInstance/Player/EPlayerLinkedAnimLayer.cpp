// Copyright EPortfolio


#include "AnimInstance/Player/EPlayerLinkedAnimLayer.h"
#include "AnimInstance/Player/EPlayerAnimInstance.h"

UEPlayerAnimInstance* UEPlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UEPlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
