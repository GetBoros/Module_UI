#include "Module_UI_Inventory.h"
#include "Module_UI.h"

// UAModule_UI_Dragged_Widget
void UAModule_UI_Dragged_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}
//-------------------------------------------------------------------------------------------------------------
void UAModule_UI_Dragged_Widget::Update_State(const UImage *image, const FVector2D desired_size) const
{
	SizeBox_Root->SetWidthOverride(desired_size.X);
	SizeBox_Root->SetHeightOverride(desired_size.Y);  // Set Widget Box Size
	Border_Root->SetBrush(image->GetBrush() );
}
//-------------------------------------------------------------------------------------------------------------




// UAModule_UI_Inventory_Slot
FReply UAModule_UI_Inventory_Slot::NativeOnMouseButtonDown(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event)
{
	Super::NativeOnMouseButtonDown(in_geometry, in_mouse_event);

	Drag_Offset = in_geometry.AbsoluteToLocal(in_mouse_event.GetScreenSpacePosition() );  // Save Drag offset

	return UWidgetBlueprintLibrary::DetectDragIfPressed(in_mouse_event, this, EKeys::LeftMouseButton).NativeReply;  // if start drag
}
//------------------------------------------------------------------------------------------------------------
void UAModule_UI_Inventory_Slot::NativeOnDragDetected(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event, UDragDropOperation *&out_operation)
{
	UAModule_UI_Dragged_Widget *drag_visual;

	// 1.0. Display Widget
	drag_visual = CreateWidget<UAModule_UI_Dragged_Widget>(GetWorld(), Drag_Widget_Class);  // Widget to show where we drag this widget
	drag_visual->Update_State(Image_Root, GetDesiredSize() );

	// 1.1. Operation Drag n Drop Init  NewDragDrop->Offset = in_geometry.AbsoluteToLocal(in_mouse_event.GetScreenSpacePosition());
	Drag_Drop_Operation = NewObject<UADrag_Drop_Operation>(this, Drag_Drop_O_Class);
	Drag_Drop_Operation->DefaultDragVisual = drag_visual;
	Drag_Drop_Operation->Payload = this;  // Is it correct?
	Drag_Drop_Operation->Pivot = EDragPivot::MouseDown;  // where the Drag Widget Visual appears while being dragged relative to the pointer performing the drag operation.
	Drag_Drop_Operation->Widget_Reference = this;  // For recreating
	Drag_Drop_Operation->Drag_Offset = Drag_Offset;  // Off set from mouse to correct add to viewport

	out_operation = Drag_Drop_Operation;  // send operation to drop

	Super::NativeOnDragDetected(in_geometry, in_mouse_event, out_operation);
}
//------------------------------------------------------------------------------------------------------------
bool UAModule_UI_Inventory_Slot::NativeOnDrop(const FGeometry &in_geometry, const FDragDropEvent &in_drag_drop_event, UDragDropOperation *in_operation)
{
	UADrag_Drop_Operation *drop_operations;
	UAModule_UI_Inventory_Slot *drop_widget_ref;
	const FSlateBrush from_drop_slot_brush = Image_Root->GetBrush();  // Save brush for dropped slot || To set in dropped slot

	drop_operations = Cast<UADrag_Drop_Operation>(in_operation);  // Get Info from other widget(slot)
	drop_widget_ref = Cast<UAModule_UI_Inventory_Slot>(drop_operations->Widget_Reference);  // check if dragged slot not something else

	Image_Root->SetBrush(drop_widget_ref->Image_Root->GetBrush() );  // Change brush to dropped Slot
	drop_widget_ref->Image_Root->SetBrush(from_drop_slot_brush);  // Change brush from dropped slot

	SetFocus();  // Set Focus to dropped widget
	Super::NativeOnDrop(in_geometry, in_drag_drop_event, in_operation);
	return true;
}
//-----------------------------------------------------------------------------------------------------------
