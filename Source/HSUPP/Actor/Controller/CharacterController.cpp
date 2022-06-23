// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"

#include "Widget/MainWidget.h"
#include "Component/InteractableAreaComponent/InteractableAreaComponent.h"
#include "Actor/NormalLevelActor/NormalLevelActor.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Widget/HomeWidget/HomeWidget.h"
#include "GameFramework/PlayerStart.h"

#include "GameMode/HGameMode.h"

ACharacterController::ACharacterController()
{
	// �������� �������Ʈ�� �ҷ��ɴϴ�.
	static ConstructorHelpers::FClassFinder<UMainWidget> MAIN_WIDGET(TEXT("WidgetBlueprint'/Game/Widget/MainWidget.MainWidget_C'"));
	if (MAIN_WIDGET.Succeeded())
	{
		BP_MainWidget = MAIN_WIDGET.Class;
	}
	
	// �ε� �������Ʈ�� �ҷ��ɴϴ�.
	static ConstructorHelpers::FClassFinder<UUserWidget> LOADING_WIDGET(TEXT("WidgetBlueprint'/Game/Widget/LoadingWidget.LoadingWidget_C'"));
	if(LOADING_WIDGET.Succeeded())
	{
		BP_LoadingWidget = LOADING_WIDGET.Class;
	}

	// Ȩ �����������Ʈ�� �ҷ��ɴϴ�.
	static ConstructorHelpers::FClassFinder<UUserWidget> HOME_WIDGET(TEXT("WidgetBlueprint'/Game/Widget/HomeWidget.HomeWidget_C'"));
	if(HOME_WIDGET.Succeeded())
		BP_HomeWidget = HOME_WIDGET.Class;
}

void ACharacterController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// ���ǿ� �����ϸ� �÷��̾�ĳ���ͷ� ������ݴϴ�.
	PlayerCharacter = Cast<APlayerCharacter>(aPawn);
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	// ���������� ������ �����ϱ����� ���� ������ Ÿ���� �о�ɴϴ�.
	ELevelActorType newLevelType = Cast<ADefaultLevelActor>(GetWorld()->GetLevelScriptActor())->GetLevelType();

	
	if (newLevelType == ELevelActorType::Normal)
	{

		// �븻 ���̶�� ��ǲ��带���Ӹ��� �����մϴ�.
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		// �����ν��Ͻ��� ��Ʈ�ѷ��� ����մϴ�.
		GetHInstance()->SetPlayerController(this);

		// �÷��̾� ���۾��͸� �ҷ��ͼ� ��ġ�� �����մϴ�.
		TArray<AActor*> playerStart;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(),FName(TEXT("StartActor")), playerStart);
		FVector newVector = Cast<APlayerStart>(playerStart[0])->GetActorLocation();
		FRotator newRotator = FRotator(0.0f,- 90.0f,0.0f);


		// ����� ��ġ�� �÷��̾� ĳ���͸� �����մϴ�.
		APlayerCharacter* playerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(newVector, newRotator);
		GetHInstance()->SetPlayerCharacter(playerCharacter);

		// �ڵ����� �����Ǵ� ��ü�� DefaultPawn �� �����ϴ� ����
		GetPawn()->Destroy();
		this->UnPossess();

		// ������ �÷��̾� ĳ���͸� �����ϰ� �����մϴ�.
		playerCharacter->PossessedBy(this);
		SetPawn(playerCharacter);
		// ��ǲ������Ʈ ���� ȣ���� ���� ���� ��������ݴϴ�.
		playerCharacter->PawnClientRestart();
		
			
		// ���������� �����ϰ� ȭ�鿡 �ٿ��ݴϴ�.
		MainWidget = CreateWidget<UMainWidget>(this, BP_MainWidget);
		MainWidget->AddToViewport();
		MainWidget->SetPlayerController(this);
		GetHInstance()->SetMainWidget(MainWidget);
	
		// �κ��丮 ���� â�� �����Ҷ� ȣ��Ǵ� �븮�ڸ� �����մϴ�.
		InventoryOpen.BindLambda([this]()
			{
				if(GetHInstance()->GetMainWidget()->GetIsInteracting()) return;
				MainWidget->CreateInventoryWnd();
			});
		  
		EquipOpen.BindLambda([this]()
			{
				if (GetHInstance()->GetMainWidget()->GetIsInteracting()) return;
				MainWidget->CreateEquipmentWnd();
			});
	
	
		PressEscButton.BindLambda([this]()
			{
				//â�� �����ִٸ� â�� ���ϴ�.
				if(MainWidget->GetWndNum() > 0)
					MainWidget->CloseFocusWnd();
				else if(MainWidget->GetIsInteracting())
					MainWidget->ClosedDialogWidget();
				// ���� â�� �������� �ʴٸ� �ɼ��� �մϴ�.
				else
					MainWidget->CreateOptionWnd();
			});
	}
	else if(newLevelType == ELevelActorType::Loading)
	{
		// �ε� ������ ���� ����ݴϴ�.
		LoadingWidget = CreateWidget(this, BP_LoadingWidget);
		LoadingWidget->AddToViewport();
	}
	else if (newLevelType == ELevelActorType::Home)
	{
		//Ȩ ������ �����ؼ� ȭ�鿡 ����ݴϴ�.
		HomeWidget = CreateWidget<UHomeWidget>(this, BP_HomeWidget);
		HomeWidget->AddToViewport();

		GetHInstance()->SetHomeWidget(HomeWidget);
		// Ȩ ������ ������ ui ��带 ����� Ŭ���Ҽ� �ֵ��� ���ݴϴ�.
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}

}

void ACharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAxis(TEXT("MouseX"), this, &ACharacterController::AddMouseX);
	InputComponent->BindAxis(TEXT("MouseY"), this, &ACharacterController::AddMouseY);

	InputComponent->BindAction(TEXT("OpenInventory"), EInputEvent::IE_Pressed, this, &ACharacterController::OpenInventoryWnd);
	InputComponent->BindAction(TEXT("OpenEquipWnd"), EInputEvent::IE_Pressed, this, &ACharacterController::OpenEquipWnd);
	InputComponent->BindAction(TEXT("CloseLastWnd"), EInputEvent::IE_Pressed, this, &ACharacterController::EscButtonEvent);
	InputComponent->BindAction(TEXT("StartInteraction"), EInputEvent::IE_Pressed, this, &ACharacterController::StartInteract);
}



void ACharacterController::AddMouseX(float axis)
{
	AddYawInput(axis);
}

void ACharacterController::AddMouseY(float axis)
{
	AddPitchInput(axis);
}

void ACharacterController::StartInteract()
{
	// ��ȣ�ۿ� ������ ���Ͱ� ���ٸ� �����Ű�� �ʽ��ϴ�.
	if(PlayerCharacter->GetInteractableActors().Num() == 0) return;

	// ���̾�α׸� �������ְ� ��ǲ��带 �������ݴϴ�.
	PlayerCharacter->GetInteractableActors()[0]->CreateDialog();
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;

}
