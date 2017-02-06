#include "godotsteam.h"
#include <steam/steam_api.h>

#include "core/io/ip_address.h"
#include "core/io/ip.h"

Steam* Steam::singleton = NULL;

Steam::Steam(){
	isInitSuccess = false;
	singleton = this;
}

Steam* Steam::get_singleton(){
	return singleton;
}

CSteamID Steam::createSteamID(uint32 steamID, int accountType){
	CSteamID cSteamID;
	if(accountType < 0 || accountType >= k_EAccountTypeMax){
		accountType = 1;
	}
	cSteamID.Set(steamID, EUniverse(k_EUniversePublic), EAccountType(accountType));
	return cSteamID;
}

///// Steamworks Functions
bool Steam::steamInit(){
	return SteamAPI_Init();
	//printf("Godot Steam initialing...\n");
	isInitSuccess = SteamAPI_Init();
	int err = FAILED;
	if(isInitSuccess){
		err = OK;
	}
	if(!SteamAPI_IsSteamRunning()){
		err = ERR_NO_CLIENT;
	}
	else if(!SteamUser()->BLoggedOn()){
		err = ERR_NO_CONNECTION;
	}
	if(err == OK && SteamUserStats() != NULL){
		// Load stats and achievements automatically
		SteamUserStats()->RequestCurrentStats();
	}
	return err;
}
bool Steam::isSteamRunning(void){
	return SteamAPI_IsSteamRunning();
}
///// Apps
bool Steam::hasOtherApp(int value){
	return SteamApps()->BIsSubscribedApp(value);
}
int Steam::getDLCCount(){
	return SteamApps()->GetDLCCount();
}
bool Steam::isDLCInstalled(int value){
	return SteamApps()->BIsDlcInstalled(value);
}
bool Steam::isSubscribedApp(int value){
	return SteamApps()->BIsSubscribedApp(value);
}
void Steam::requestAppProofOfPurchaseKey(int value){
	return SteamApps()->RequestAppProofOfPurchaseKey(value);
}
///// Friends
int Steam::getFriendCount(){
	return SteamFriends()->GetFriendCount(0x04);
}
String Steam::getPersonaName(){
	return SteamFriends()->GetPersonaName();
}
String Steam::getFriendPersonaName(int steamID){
	if(SteamFriends() != NULL && steamID > 0){
		CSteamID friendID = createSteamID(steamID);
		bool isDataLoading = SteamFriends()->RequestUserInformation(friendID, true);
		if(!isDataLoading){
			return SteamFriends()->GetFriendPersonaName(friendID);
		}
	}
	return "";
}
void Steam::setGameInfo(const String& s_key, const String& s_value){
	// Rich presence data is automatically shared between friends in the same game
	// Each user has a set of key/value pairs, up to 20 can be set
	// Two magic keys (status, connect)
	// setGameInfo() to an empty string deletes the key
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->SetRichPresence(s_key.utf8().get_data(), s_value.utf8().get_data());
}
void Steam::clearGameInfo(){
	if(SteamFriends() == NULL){
		return;
	}
	SteamFriends()->ClearRichPresence();
}
void Steam::inviteFriend(int steamID, const String& conString){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID friendID = createSteamID(steamID);
	SteamFriends()->InviteUserToGame(friendID, conString.utf8().get_data());
}
void Steam::setPlayedWith(int steamID){
	if(SteamFriends() == NULL){
		return;
	}
	CSteamID friendID = createSteamID(steamID);
	SteamFriends()->SetPlayedWith(friendID);
}
Array Steam::getRecentPlayers(){
	if(SteamFriends() == NULL){
		return Array();
	}
	int rCount = SteamFriends()->GetCoplayFriendCount();
	Array recents;
	for(int i=0; i<rCount; i++){
		CSteamID rPlayerID = SteamFriends()->GetCoplayFriend(i);
		if(SteamFriends()->GetFriendCoplayGame(rPlayerID) == SteamUtils()->GetAppID()){
			Dictionary rPlayer;
			String rName = SteamFriends()->GetFriendPersonaName(rPlayerID);
			int rStatus = SteamFriends()->GetFriendPersonaState(rPlayerID);
			rPlayer["id"] = rPlayerID.GetAccountID();
			rPlayer["name"] = rName;
			rPlayer["status"] = rStatus;
			recents.append(rPlayer);
		}
	}
	return recents;
}
///// Signals
void Steam::_server_connected(SteamServersConnected_t* conData){
	emit_signal("connection_changed", true);
}
void Steam::_server_disconnected(SteamServersDisconnected_t* conData){
	emit_signal("connection_changed", false);
}
void Steam::_join_requested(GameRichPresenceJoinRequested_t* callData){
	int steamID = callData->m_steamIDFriend.GetAccountID();
	String con_string = callData->m_rgchConnect;
	emit_signal("join_requested", steamID, con_string);
}
///// Users
// setServerInfo is removed as version 2.1.2 of Godot breaks it
//void Steam::setServerInfo(const String& server_ip, int port){
//	if(SteamUser() == NULL){
//		return;
//	}
//	// Resolve address and convert it to INT (IP_Address) union
//	IP_Address addr(IP::get_singleton()->resolve_hostname(server_ip));
//	// Bytes are in the wrong order, need to swap them
//	for(int i=0; i<2; i++){
//		uint8 temp = addr.field[i];
//		addr.field[i] = addr.field[3-i];
//		addr.field[3-i] = temp;
//	}
//	// Create server ID based on user's ID
//	CSteamID gameserverID = SteamUser()->GetSteamID();
//	SteamUser()->AdvertiseGame(gameserverID, addr.host, port);
//}
int Steam::getSteamID(){
	CSteamID cSteamID = SteamUser()->GetSteamID();
	return cSteamID.ConvertToUint64();
}
bool Steam::loggedOn(){
	return SteamUser()->BLoggedOn();
}
int Steam::getPlayerSteamLevel(){
	return SteamUser()->GetPlayerSteamLevel(); 
}
///// User Stats
bool Steam::clearAchievement(const String& s_key){
	return SteamUserStats()->ClearAchievement(s_key.utf8().get_data());
}
bool Steam::getAchievement(const String& s_key){
	bool achieved = false;
	SteamUserStats()->GetAchievement(s_key.utf8().get_data(), &achieved);
	return achieved;
}
float Steam::getStatFloat(const String& s_key){
	float statval = 0;
	SteamUserStats()->GetStat(s_key.utf8().get_data(), &statval);
	return statval;
}
int Steam::getStatInt(const String& s_key){
	int32 statval = 0;
	SteamUserStats()->GetStat(s_key.utf8().get_data(), &statval);
	return statval;
}
bool Steam::resetAllStats(bool bAchievementsToo){
	SteamUserStats()->ResetAllStats(bAchievementsToo);
	return SteamUserStats()->StoreStats();
}
bool Steam::requestCurrentStats(){
	return SteamUserStats()->RequestCurrentStats();
}
bool Steam::setAchievement(const String& s_key){
	return SteamUserStats()->SetAchievement(s_key.utf8().get_data());
}
bool Steam::setStatFloat(const String& s_key, float value){
	return SteamUserStats()->SetStat(s_key.utf8().get_data(), value);
}
bool Steam::setStatInt(const String& s_key, int value){
	return SteamUserStats()->SetStat(s_key.utf8().get_data(), value);
}
bool Steam::storeStats(){
	return SteamUserStats()->StoreStats();
}
///// Utils
String Steam::getIPCountry(){
	return SteamUtils()->GetIPCountry();
}
bool Steam::isOverlayEnabled(){
	return SteamUtils()->IsOverlayEnabled();
}
String Steam::getSteamUILanguage(){
	return SteamUtils()->GetSteamUILanguage();
}
int Steam::getAppID(){
	if(SteamUtils() == NULL){
		return 0;
	}
	return SteamUtils()->GetAppID();
}

void Steam::_bind_methods()
{
	ObjectTypeDB::bind_method("steamInit", &Steam::steamInit);
	ObjectTypeDB::bind_method("isSteamRunning", &Steam::isSteamRunning);
	// Apps Bind Methods
	ObjectTypeDB::bind_method("hasOtherApp", &Steam::hasOtherApp);
	ObjectTypeDB::bind_method("getDLCCount", &Steam::getDLCCount);
	ObjectTypeDB::bind_method("isDLCInstalled", &Steam::isDLCInstalled);
	ObjectTypeDB::bind_method("requestAppProofOfPurchaseKey", &Steam::requestAppProofOfPurchaseKey);
	// Friends Bind Methods
	ObjectTypeDB::bind_method("getFriendCount", &Steam::getFriendCount);
	ObjectTypeDB::bind_method("getPersonaName", &Steam::getPersonaName);
	ObjectTypeDB::bind_method("getFriendPersonaName", &Steam::getFriendPersonaName);
	ObjectTypeDB::bind_method(_MD("setGameInfo", "key", "value"), &Steam::setGameInfo);
	ObjectTypeDB::bind_method(_MD("clearGameInfo"), &Steam::clearGameInfo);
	ObjectTypeDB::bind_method(_MD("inviteFriend", "steam_id", "connect_string"), &Steam::inviteFriend);
	ObjectTypeDB::bind_method(_MD("setPlayedWith", "steam_id"), &Steam::setPlayedWith);
	ObjectTypeDB::bind_method("getRecentPlayers", &Steam::getRecentPlayers);
	// User Bind Methods
//	ObjectTypeDB::bind_method(_MD("setServerInfo", "server_ip", "port"), &Steam::setServerInfo);
	ObjectTypeDB::bind_method("getSteamID", &Steam::getSteamID);
	ObjectTypeDB::bind_method("loggedOn", &Steam::loggedOn);
	ObjectTypeDB::bind_method("getPlayerSteamLevel", &Steam::getPlayerSteamLevel);
	// User Stats Bind Methods
	ObjectTypeDB::bind_method("clearAchievement", &Steam::clearAchievement);
	ObjectTypeDB::bind_method("getAchievement", &Steam::getAchievement);
	ObjectTypeDB::bind_method("getStatFloat", &Steam::getStatFloat);
	ObjectTypeDB::bind_method("getStatInt", &Steam::getStatInt);
	ObjectTypeDB::bind_method("resetAllStats", &Steam::resetAllStats);
	ObjectTypeDB::bind_method("requestCurrentStats", &Steam::requestCurrentStats);
	ObjectTypeDB::bind_method("setAchievement", &Steam::setAchievement);
	ObjectTypeDB::bind_method("setStatFloat", &Steam::setStatFloat);
	ObjectTypeDB::bind_method("setStatInt", &Steam::setStatInt);
	ObjectTypeDB::bind_method("storeStats", &Steam::storeStats);
	// Utils Bind Methods
	ObjectTypeDB::bind_method("getIPCountry", &Steam::getIPCountry);
	ObjectTypeDB::bind_method("isOverlayEnabled", &Steam::isOverlayEnabled);
	ObjectTypeDB::bind_method("getSteamUILanguage", &Steam::getSteamUILanguage);
	ObjectTypeDB::bind_method("getAppID", &Steam::getAppID);
	// Signals
	ADD_SIGNAL(MethodInfo("connection_changed", PropertyInfo(Variant::BOOL, "connected")));
	ADD_SIGNAL(MethodInfo("join_requested", PropertyInfo(Variant::INT, "from"), PropertyInfo(Variant::STRING, "connect_string")));
	// Status constants
	BIND_CONSTANT(OFFLINE);		// 0
	BIND_CONSTANT(ONLINE);		// 1
	BIND_CONSTANT(BUSY);		// 2
	BIND_CONSTANT(AWAY);		// 3
	BIND_CONSTANT(SNOOZE);		// 4
	BIND_CONSTANT(LF_TRADE);	// 5
	BIND_CONSTANT(LF_PLAY);		// 6
	BIND_CONSTANT(NOT_OFFLINE); // Custom
	BIND_CONSTANT(ALL); 		// Custom
	// Initialization errors
	BIND_CONSTANT(ERR_NO_CLIENT);
	BIND_CONSTANT(ERR_NO_CONNECTION);
}

Steam::~Steam(){
	if(isInitSuccess){
		//printf("Godot Steam exiting\n");
		SteamUserStats()->StoreStats();
		SteamAPI_Shutdown();
	}
	singleton = NULL;
}
