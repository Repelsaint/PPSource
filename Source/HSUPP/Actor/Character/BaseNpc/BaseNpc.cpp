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
	
	// NPC ���̾�α� â �������Ʈ�� �ҷ��ɴϴ�.
	static ConstructorHelpers::FClassFinder<UNpcDialog> BP_NPC_DIALOG(TEXT("WidgetBlueprint'/Game/Widget/NPC_DIalog/BP_NPC_Dialog.BP_NPC_Dialog_C'"));

	if (BP_NPC_DIALOG.Succeeded())
		BP_NpcDialog = BP_NPC_DIALOG.Class;

	// ��ȣ�ۿ� ������Ʈ�� �����մϴ�.
	InteractableArea = CreateDefaultSubobject<UInteractableAreaComponent>(TEXT("SPHERE_COMPONENT"));
	InteractableArea->SetupAttachment(GetRootComponent());



}

// Called when the game starts or when spawned
void ABaseNpc::BeginPlay()
{
	Super::BeginPlay();

	// ��ȣ�ۿ� ������Ʈ�� ��ȣ�ۿ�â ���� �޼��带 ���ε��մϴ�.
	InteractableArea->CreateInteractWnd.BindLambda([this]()
	{
	 // �̹� ��ȣ�ۿ����̶�� �������� �ʽ��ϴ�.
		if(GetHInstance()->GetMainWidget()->GetIsInteracting()) return;

		// NPC ���̾�α׸� �����ϰ� ������ NPC�� ������ �ʱ�ȭ���ݴϴ�.
		UNpcDialog* newDialog = CreateWidget<UNpcDialog>(GetHInstance()->GetPlayerController(),BP_NpcDialog);
		newDialog->InitializeDialog(NpcName,NpcCode);

		//������ ���̾�α׸� ���������� ���̰� ���ü��� â������ ó���� ���ݴϴ�.
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

