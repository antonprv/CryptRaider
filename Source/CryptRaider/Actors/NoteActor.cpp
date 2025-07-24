// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "NoteActor.h"

#include "PressurePlateActor.h"

#include "Blueprint/UserWidget.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"

#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"

#include "Screamers/ScreamerActor.h"

// Sets default values
ANoteActor::ANoteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Note Mesh"));
	RootComponent = NoteMesh;
	NoteMesh->SetHiddenInGame(true);
	NoteMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Note pickup area"));
	CollisionSphere->SetupAttachment(NoteMesh);

	FloatingText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Helper Text"));
	FloatingText->SetupAttachment(NoteMesh);
	FloatingText->SetVisibility(false);
	FloatingText->SetHiddenInGame(true);
	FloatingText->SetCollisionResponseToAllChannels(ECR_Ignore);

	CollisionSphere->SetGenerateOverlapEvents(true);
	
	CollisionSphere->SetSphereRadius(220.f);
	CollisionSphere->SetLineThickness(3.f);
	
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ANoteActor::OnPlayerEnterPickupArea);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ANoteActor::OnPlayerExitPickupArea);

}

// Called when the game starts or when spawned
void ANoteActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> RawScreamerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScreamerActor::StaticClass(), RawScreamerActors);

	ScreamerActors.Empty();
    for (AActor* Actor : RawScreamerActors)
    {
        ScreamerActors.Add(Actor);  // This implicitly converts to TWeakObjectPtr
        
        if (AScreamerActor* ScreamerActor = Cast<AScreamerActor>(Actor))
        {
            ScreamerActor->OnPlayerStartedScreamer.AddDynamic(this, &ANoteActor::OnNoteBecomeVisible);
            ScreamerActor->OnScreamerDone.AddDynamic(this, &ANoteActor::OnNoteBecomeInvisible);
        }
    }
}
}

void ANoteActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CollisionSphere->OnComponentBeginOverlap.RemoveDynamic(this, &ANoteActor::OnPlayerEnterPickupArea);
	CollisionSphere->OnComponentEndOverlap.RemoveDynamic(this, &ANoteActor::OnPlayerExitPickupArea);

	// Safe iteration with weak pointers
	for (auto It = ScreamerActors.CreateIterator(); It; ++It)
	{
		if (AActor* Actor = It->Get())  // Only processes valid actors
		{
			if (AScreamerActor* ScreamerActor = Cast<AScreamerActor>(Actor))
			{
				ScreamerActor->OnPlayerStartedScreamer.RemoveDynamic(this, &ANoteActor::OnNoteBecomeVisible);
				ScreamerActor->OnScreamerDone.RemoveDynamic(this, &ANoteActor::OnNoteBecomeInvisible);
			}
		}
		else
		{
			// Remove invalid entries
			It.RemoveCurrent();
		}
	}
    
	// Always clean up input
	UnbindInteractionInput();
	
	Super::EndPlay(EndPlayReason);
}

void ANoteActor::OnNoteBecomeInvisible()
{
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		bCanPlayerSeeNote = false;
		NoteMesh->SetHiddenInGame(true);
	});
}

void ANoteActor::OnNoteBecomeVisible(
	EMusicTriggerType MusicToPlay)
{
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		bCanPlayerSeeNote = true;
		NoteMesh->SetHiddenInGame(false);
	});
}

void ANoteActor::OnPlayerEnterPickupArea(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		OnPlayerInNoteArea.Broadcast(true);

		if (bCanPlayerSeeNote)
		{
			FloatingText->SetVisibility(true);
			FloatingText->SetHiddenInGame(false);
		}
		else if (!bCanPlayerSeeNote)
		{
			FloatingText->SetVisibility(false);
			FloatingText->SetHiddenInGame(true);
		}

		if (ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor))
		{
			PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
			if (PlayerController)
			{
				BindInteractionInput();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed To cast to PlayerController"))
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Cast to charcter"))
		}
		
	}
}

void ANoteActor::OnPlayerExitPickupArea(
	 UPrimitiveComponent* OverlappedComponent,
	 AActor* OtherActor,
	 UPrimitiveComponent* OtherComp,
	 int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		OnPlayerInNoteArea.Broadcast(false);
		FloatingText->SetVisibility(false);
		FloatingText->SetHiddenInGame(true);
		UnbindInteractionInput();
	}
}

void ANoteActor::SetPickedUp()
{
	OnPlayerPickedUpNote.Broadcast();
}

void ANoteActor::BindInteractionInput()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if (!InputComponent)
	{
		InputComponent = NewObject<UInputComponent>(this);
		InputComponent->RegisterComponent();
	}

	EnableInput(PlayerController);
	InputComponent->BindKey(EKeys::E, IE_Pressed, this, &ANoteActor::OnInteractPressed);
}

void ANoteActor::UnbindInteractionInput()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if (InputComponent)
	{
		InputComponent->ClearActionBindings();
		InputComponent->ClearBindingValues();
	}

	DisableInput(PlayerController);
}

void ANoteActor::OnInteractPressed()
{
	if (!GetWorld()->GetFirstPlayerController() || bIsWidgetCreated)
	{
		return;
	}
	
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("NoteActor: Failed to load widget class."));
		return;
	}

	if (UUserWidget* WBP_Note = CreateWidget<UUserWidget>(PlayerController, WidgetClass))
	{
		if (bCanPlayerSeeNote)
		{
			WBP_Note->AddToViewport();
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);

			bIsWidgetCreated = true;
		}
	}
	else if (!WBP_Note)
	{
		UE_LOG(LogTemp, Error, TEXT("NoteActor: Failed to create widget instance."));
	}

	Destroy();
}

