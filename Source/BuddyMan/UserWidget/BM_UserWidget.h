// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"
#include "BM_UserWidget.generated.h"

UCLASS()
class BUDDYMAN_API UBM_UserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBM_UserWidget(const FObjectInitializer& ObjectInitializer);

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};