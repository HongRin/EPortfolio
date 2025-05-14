// Copyright EPortfolio


#include "EServerRow.h"
#include "Components/Button.h"
#include "EMenu.h"

void UEServerRow::RowButtonClicked()
{
	Parent->SelectIndex(Index);
}

void UEServerRow::Setup(UEMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	Button_Row->OnClicked.AddDynamic(this, &UEServerRow::RowButtonClicked);
}

void UEServerRow::SetRowColor()
{
	if (Selected)
	{
		Button_Row->SetColorAndOpacity(FLinearColor(1.0f, 0.2f, 1.0f));
	}
	else
	{
		Button_Row->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
	}
}
