#include "EmbeddedAssets.h"

UEmbeddedAssets::UEmbeddedAssets(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (embed_list != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EmbeddedAssets: only should ever be one instance of this!"));
	}

	embed_list = NewObject<UEmbedList>();
}

UEmbeddedAssets::~UEmbeddedAssets()
{	
}
