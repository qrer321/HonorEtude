#pragma once																															 
																																		 
#include "ThreadHandlerChatMessage.h"														
#include "ThreadHandlerPlayerUpdateMessage.h"														
#include "ThreadHandlerLoginResultMessage.h"																
#include "ThreadHandlerJoinResultMessage.h"																
#include "ThreadHandlerCreateCharacterResultMessage.h"																
#include "ThreadHandlerSelectCharacterResultMessage.h"																
#include "ThreadHandlerInsertSectionResultMessage.h"																
#include "ThreadHandlerCharacterListMessage.h"																
#include "ThreadHandlerServerDestroyMessage.h"																
#include "ThreadHandlerPlayerDestroyMessage.h"																
#include "ThreadHandlerObjectDestroyMessage.h"																
#include "ThreadHandlerPlayersUpdateMessage.h"																
#include "ThreadHandlerEnemyUpdateMessage.h"																
																																		 
template <typename MessageHandler, typename MessageType>																				 
void OnMessageProcess(std::shared_ptr<GameServerMessage> Message, UWorld* World)														 
{																																		 
	std::shared_ptr<MessageType> ConvertMessage = std::static_pointer_cast<MessageType>(MoveTemp(Message));								 
	if (nullptr == ConvertMessage)																										 
	{																																	 
		return;																															 
	}																																	 
																																		 
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(World->GetGameInstance());								 
																																		 
	MessageHandler Handler = MessageHandler();																							 
	Handler.Initialize(MoveTempIfPossible(ConvertMessage), GameInstance, World);														 
	Handler.Start();																													 
}																																		 
																																		 
inline void AddGlobalHandler(Dispatcher& Dis, UWorld* World)																			 
{																																		   
	Dis.AddHandler(MessageType::Chat,
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)
	               {
		               OnMessageProcess<ThreadHandlerChatMessage, ChatMessage>(MoveTemp(GameServerMessage), World);
	               });

	Dis.AddHandler(MessageType::PlayerUpdate,
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)
	               {
		               OnMessageProcess<ThreadHandlerPlayerUpdateMessage, PlayerUpdateMessage>(MoveTemp(GameServerMessage), World);
	               });
																																	
	Dis.AddHandler(MessageType::LoginResult,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerLoginResultMessage, LoginResultMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::JoinResult,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerJoinResultMessage, JoinResultMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::CreateCharacterResult,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerCreateCharacterResultMessage, CreateCharacterResultMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::SelectCharacterResult,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerSelectCharacterResultMessage, SelectCharacterResultMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::InsertSectionResult,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerInsertSectionResultMessage, InsertSectionResultMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::CharacterList,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerCharacterListMessage, CharacterListMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::ServerDestroy,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerServerDestroyMessage, ServerDestroyMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::PlayerDestroy,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerPlayerDestroyMessage, PlayerDestroyMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::ObjectDestroy,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerObjectDestroyMessage, ObjectDestroyMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::PlayersUpdate,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerPlayersUpdateMessage, PlayersUpdateMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::EnemyUpdate,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerEnemyUpdateMessage, EnemyUpdateMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
}																																  		 
