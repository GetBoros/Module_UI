#include "Module_UI_HUD.h"
#include "Module_UI.h"

// AAModule_UI_HUD 
AAModule_UI_HUD::AAModule_UI_HUD()
 : Menu_Main(0), Inventory(0), Menu_Main_Widget(0), Inventory_Widget(0)
{

}
//-----------------------------------------------------------------------------------------------------------
void AAModule_UI_HUD::BeginPlay()
{
    if (Menu_Main != 0)  // Create Menu Main, maybe set to manual func
		(Menu_Main_Widget = CreateWidget<UUserWidget>(GetWorld(), Menu_Main) )->AddToViewport(0);

	if (Inventory.IsValid() == true)  // Try to a sync load widget and create widget, but don`t add to viewport
		Inventory_Load_Stream();
	else
		UAssetManager::GetStreamableManager().RequestAsyncLoad(Inventory.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AAModule_UI_HUD::Inventory_Load_Stream) );

	Super::BeginPlay();  // Can add to view port Inventory or else
}
//-----------------------------------------------------------------------------------------------------------
void AAModule_UI_HUD::Inventory_Load_Stream()
{
    UClass *loaded_class = Inventory.Get();

    if (loaded_class != 0)
		Inventory_Widget = CreateWidget<UUserWidget>(GetWorld(), loaded_class);
}
//-----------------------------------------------------------------------------------------------------------
void AAModule_UI_HUD::Inventory_Handle(const int equipment, const int slots)
{

}
//-----------------------------------------------------------------------------------------------------------
void AAModule_UI_HUD::Inventory_Toggle()
{
	if (Inventory_Widget->IsVisible() )  // Toggle Inventory visibility
		Inventory_Widget->SetVisibility(ESlateVisibility::Hidden);
	else
		Inventory_Widget->SetVisibility(ESlateVisibility::Visible);

	if (Inventory_Widget->IsInViewport() != true)
		Inventory_Widget->AddToViewport();
}
//-----------------------------------------------------------------------------------------------------------
