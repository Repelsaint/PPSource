// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcDialog.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Widget/MainWidget.h"




UNpcDialog::UNpcDialog(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UNpcDialog::NativeConstruct()
{
	Super::NativeConstruct();
	Cast<UCanvasPanelSlot>(this->Slot)->SetSize(FVector2D(1820.0f,250.0f));
	
	Cast<UCanvasPanelSlot>(this->Slot)->SetAnchors(FAnchors(0.5f,1.0f));
	Cast<UCanvasPanelSlot>(this->Slot)->SetAlignment(FVector2D(0.5f,1.0f));
	Cast<UCanvasPanelSlot>(this->Slot)->SetPosition(FVector2D(0.0f,-10.0f));

	Button_Close->OnClicked.AddDynamic(this,&UNpcDialog::WidgetClosed);
	Button_Function->OnClicked.AddDynamic(this, &UNpcDialog::TryNpcFunction);
}

void UNpcDialog::InitializeDialog(FText npcName, int npcCode)
{
	NpcCode = npcCode;
	TextBlock_NameBox->SetText(npcName);
	FString jsonName = TEXT("NPC_") + FString::FromInt(npcCode);
	GetHInstance()->LoadTArrayStringJson(jsonName,TEXT("Comment"), CommentArray);

	if(CommentArray.Num() > 1)
	{
		int arrayIndex = FMath::RandRange(0,(CommentArray.Num()-1));
		
		TextBlock_CommentBox->SetText(FText::FromString(CommentArray[arrayIndex]));
	}
	else if( CommentArray.Num() == 1)
		TextBlock_CommentBox->SetText(FText::FromString(CommentArray[0]));
	else
	{
		TextBlock_CommentBox->SetText(FText::FromString(TEXT("Error!")));
	}	
	
}

void UNpcDialog::WidgetClosed()
{
	GetHInstance()->GetMainWidget()->ClosedDialogWidget();
}

void UNpcDialog::TryNpcFunction()
{
	
	GetHInstance()->GetMainWidget()->CreateNewShopWnd(NpcCode);
}
