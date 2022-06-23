// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Blueprint/UserWidget.h"
#include "NpcDialog.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnWidgetClosed)

UCLASS()
class HSUPP_API UNpcDialog : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_NameBox;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_CommentBox;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Function;

	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Close;

	TArray<FString> CommentArray;
	int NpcCode;

public:
	FOnWidgetClosed OnWidgetClosed;

public:
	UNpcDialog(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void InitializeDialog(FText npcName,int npcCode);
	
	UFUNCTION()
	void WidgetClosed();

	UFUNCTION()
	void TryNpcFunction();

};
