// Copyright EPortfolio


#include "HUD/ESniperScopeWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "ELogHelpers.h"

void UESniperScopeWidget::PlayZoomIn(bool bIsAimming)
{
	if (bIsAimming)
	{
		PlayAnimation(Animation_ZoomIn);
	}
	else
	{
		PlayAnimationReverse(Animation_ZoomIn);
	}
}
