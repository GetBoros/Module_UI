#include "Module_UI_HUD.h"

//------------------------------------------------------------------------------------------------------------
FReply UModule_UI_HUD::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // Получаем координаты клика в локальных координатах виджета
    const FVector2D mouse_pos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    // Логируем координаты клика
    UE_LOG(LogTemp, Warning, TEXT("Mouse Clicked at: X: %f, Y: %f"), mouse_pos.X, mouse_pos.Y);

    return FReply::Handled(); // Сообщаем, что событие обработано
}
//------------------------------------------------------------------------------------------------------------
FReply UModule_UI_HUD::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // Получаем координаты мыши в глобальных координатах экрана
    const FVector2D mouse_pos = InMouseEvent.GetScreenSpacePosition();

    // Логируем координаты мыши
    UE_LOG(LogTemp, Warning, TEXT("Mouse Moved at: X: %f, Y: %f"), mouse_pos.X, mouse_pos.Y);

    return FReply::Unhandled(); // Возвращаем Unhandled, чтобы событие передавалось дальше
}
//------------------------------------------------------------------------------------------------------------
