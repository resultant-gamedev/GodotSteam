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
///// Users
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
	return SteamUserStats()->ResetAllStats(bAchievementsToo);
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
	// User Bind Methods
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
	
	BIND_CONSTANT(OFFLINE);		// 0
	BIND_CONSTANT(ONLINE);		// 1
	BIND_CONSTANT(BUSY);		// 2
	BIND_CONSTANT(AWAY);		// 3
	BIND_CONSTANT(SNOOZE);		// 4
	BIND_CONSTANT(LF_TRADE);	// 5
	BIND_CONSTANT(LF_PLAY);		// 6
	BIND_CONSTANT(NOT_OFFLINE); // Custom
	BIND_CONSTANT(ALL); 		// Custom
	
	BIND_CONSTANT(ERR_NO_CLIENT);
	BIND_CONSTANT(ERR_NO_CONNECTION);
}

Steam::~Steam(){
	if(isInitSuccess){
		//printf("Godot Steam exiting\n");
		SteamAPI_Shutdown();
	}
	singleton = NULL;
}