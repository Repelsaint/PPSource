// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNpc.h"
#include "Component/InteractableAreaComponent/InteractableAreaComponent.h"
#include "Widget/NpcDialog/NpcDialog.h"
#include "Widget/MainWidget.h"
#include "Components/CanvasPanel.h"

// Sets default values
ABaseNpc::ABaseNpc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// NPC 다이얼로그 창 블루프린트를 불러옵니다.
	static ConstructorHelpers::FClassFinder<UNpcDialog> BP_NPC_DIALOG(TEXT("WidgetBlueprint'/Game/Widget/NPC_DIalog/BP_NPC_Dialog.BP_NPC_Dialog_C'"));

	if (BP_NPC_DIALOG.Succeeded())
		BP_NpcDialog = BP_NPC_DIALOG.Class;

	// 상호작용 컴포넌트를 생성합니다.
	InteractableArea = CreateDefaultSubobject<UInteractableAreaComponent>(TEXT("SPHERE_COMPONENT"));
	InteractableArea->SetupAttachment(GetRootComponent());



}

// Called when the game starts or when spawned
void ABaseNpc::BeginPlay()
{
	Super::BeginPlay();

	// 상호작용 컴포넌트에 상호작용창 생성 메서드를 바인딩합니다.
	InteractableArea->CreateInteractWnd.BindLambda([this]()
	{
	 // 이미 상호작용중이라면 생성하지 않습니다.
		if(GetHInstance()->GetMainWidget()->GetIsInteracting()) return;

		// NPC 다이얼로그를 생성하고 현재의 NPC의 정보로 초기화해줍니다.
		UNpcDialog* newDialog = CreateWidget<UNpcDialog>(GetHInstance()->GetPlayerController(),BP_NpcDialog);
		newDialog->InitializeDialog(NpcName,NpcCode);

		//생성한 다이얼로그를 메인위젯에 붙이고 가시성과 창에대한 처리를 해줍니다.
		GetHInstance()->GetMainWidget()->CreateDialog(newDialog);

	}
	);

}

// Called every frame
void ABaseNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseNpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

