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
	// 메인위젯 블루프린트를 불러옵니다.
	static ConstructorHelpers::FClassFinder<UMainWidget> MAIN_WIDGET(TEXT("WidgetBlueprint'/Game/Widget/MainWidget.MainWidget_C'"));
	if (MAIN_WIDGET.Succeeded())
	{
		BP_MainWidget = MAIN_WIDGET.Class;
	}
	
	// 로딩 블루프린트를 불러옵니다.
	static ConstructorHelpers::FClassFinder<UUserWidget> LOADING_WIDGET(TEXT("WidgetBlueprint'/Game/Widget/LoadingWidget.LoadingWidget_C'"));
	if(LOADING_WIDGET.Succeeded())
	{
		BP_LoadingWidget = LOADING_WIDGET.Class;
	}

	// 홈 위젯블루프린트를 불러옵니다.
	static ConstructorHelpers::FClassFinder<UUserWidget> HOME_WIDGET(TEXT("WidgetBlueprint'/Game/Widget/HomeWidget.HomeWidget_C'"));
	if(HOME_WIDGET.Succeeded())
		BP_HomeWidget = HOME_WIDGET.Class;
}

void ACharacterController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// 빙의에 성공하면 플레이어캐릭터로 등록해줍니다.
	PlayerCharacter = Cast<APlayerCharacter>(aPawn);
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	// 레벨에따른 위젯을 구분하기위해 현제 레벨의 타입을 읽어옵니다.
	ELevelActorType newLevelType = Cast<ADefaultLevelActor>(GetWorld()->GetLevelScriptActor())->GetLevelType();

	
	if (newLevelType == ELevelActorType::Normal)
	{

		// 노말 맵이라면 인풋모드를게임모드로 지정합니다.
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		// 게임인스턴스에 컨트롤러를 등록합니다.
		GetHInstance()->SetPlayerController(this);

		// 플레이어 시작액터를 불러와서 위치를 저장합니다.
		TArray<AActor*> playerStart;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(),FName(TEXT("StartActor")), playerStart);
		FVector newVector = Cast<APlayerStart>(playerStart[0])->GetActorLocation();
		FRotator newRotator = FRotator(0.0f,- 90.0f,0.0f);


		// 저장된 위치로 플레이어 캐릭터를 생성합니다.
		APlayerCharacter* playerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(newVector, newRotator);
		GetHInstance()->SetPlayerCharacter(playerCharacter);

		// 자동으로 생성되는 구체의 DefaultPawn 을 삭제하는 구문
		GetPawn()->Destroy();
		this->UnPossess();

		// 생성한 플레이어 캐릭터를 소유하고 조종합니다.
		playerCharacter->PossessedBy(this);
		SetPawn(playerCharacter);
		// 인풋컴포넌트 등의 호출을 위해 폰을 재시작해줍니다.
		playerCharacter->PawnClientRestart();
		
			
		// 메인위젯을 생성하고 화면에 붙여줍니다.
		MainWidget = CreateWidget<UMainWidget>(this, BP_MainWidget);
		MainWidget->AddToViewport();
		MainWidget->SetPlayerController(this);
		GetHInstance()->SetMainWidget(MainWidget);
	
		// 인벤토리 등의 창을 오픈할때 호출되는 대리자를 설정합니다.
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
				//창이 켜져있다면 창을 끕니다.
				if(MainWidget->GetWndNum() > 0)
					MainWidget->CloseFocusWnd();
				else if(MainWidget->GetIsInteracting())
					MainWidget->ClosedDialogWidget();
				// 만약 창이 켜져있지 않다면 옵션을 켭니다.
				else
					MainWidget->CreateOptionWnd();
			});
	}
	else if(newLevelType == ELevelActorType::Loading)
	{
		// 로딩 위젯을 만들어서 띄워줍니다.
		LoadingWidget = CreateWidget(this, BP_LoadingWidget);
		LoadingWidget->AddToViewport();
	}
	else if (newLevelType == ELevelActorType::Home)
	{
		//홈 위젯을 생성해서 화면에 띄워줍니다.
		HomeWidget = CreateWidget<UHomeWidget>(this, BP_HomeWidget);
		HomeWidget->AddToViewport();

		GetHInstance()->SetHomeWidget(HomeWidget);
		// 홈 위젯이 켜지면 ui 모드를 띄워서 클릭할수 있도록 해줍니다.
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
	// 상호작용 가능한 액터가 없다면 실행시키지 않습니다.
	if(PlayerCharacter->GetInteractableActors().Num() == 0) return;

	// 다이얼로그를 생성해주고 인풋모드를 변경해줍니다.
	PlayerCharacter->GetInteractableActors()[0]->CreateDialog();
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;

}
