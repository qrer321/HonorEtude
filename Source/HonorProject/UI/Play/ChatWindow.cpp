// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWindow.h"
#include "ChatMessageWidget.h"
#include "ChatMessageObject.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"
#include "HonorProject/Message/Messages.h"

void UChatWindow::NativeConstruct()
{
	Super::NativeConstruct();

	m_MessageListView = Cast<UListView>(GetWidgetFromName(TEXT("MessageList")));

	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetGameInstance());
	GameInstance->m_ChatWindow = this;
}

void UChatWindow::SendChat(FString Text, ETextCommit::Type CommitType)
{
	if (ETextCommit::Type::OnEnter != CommitType)
	{
		return;
	}

	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetGameInstance());
	
	UChatMessageObject* ChatMessageObject = NewObject<UChatMessageObject>();
	ChatMessageObject->SetID(GameInstance->m_UserID);
	ChatMessageObject->SetMessage(Text);

	AddMessage(ChatMessageObject);
	
	ChatMessage Message;

	std::string ID;
	std::string SendMessage;

	const FString ObjectID = ChatMessageObject->GetID();
	const FString ObjectMessage = ChatMessageObject->GetMessage();
	UClientBlueprintFunctionLibrary::FStringToUTF8(ObjectID, ID);
	UClientBlueprintFunctionLibrary::FStringToUTF8(ObjectMessage, SendMessage);

	Message.m_ID = ID;
	Message.m_Message = SendMessage;

	GameServerSerializer Serializer;
	Message.Serialize(Serializer);

	GameInstance->Send(Serializer.GetData());
}

void UChatWindow::AddNewMessage(UObject* Object, UUserWidget* UserWidget)
{
	if (false == IsValid(Object))
	{
		return;
	}

	if (false == IsValid(UserWidget))
	{
		return;
	}
	
	UChatMessageObject* MessageObject = Cast<UChatMessageObject>(Object);
	if (false == IsValid(MessageObject))
	{
		return;
	}

	UChatMessageWidget* MessageWidget = Cast<UChatMessageWidget>(UserWidget);
	if (false == IsValid(MessageWidget))
	{
		return;
	}

	MessageWidget->SetChatString(MessageObject->GetID() + TEXT(" : ") + MessageObject->GetMessage());
}

void UChatWindow::AddMessage(UChatMessageObject* ChatMessageObject)
{
	m_MessageListView->AddItem(ChatMessageObject);
	m_MessageListView->SetScrollOffset(m_MessageListView->GetNumItems() * 50.f);
}

void UChatWindow::AddMessage(const FString& ID, const FString& Message)
{
	UChatMessageObject* MessageObject = NewObject<UChatMessageObject>();

	MessageObject->SetID(ID);
	MessageObject->SetMessage(Message);

	AddMessage(MessageObject);
}
