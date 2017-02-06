#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

#include <steam/steam_api.h>

#include "object.h"
#include "dictionary.h"		// Contains array.h as well

class Steam: public Object
{
public:
	enum {
		OFFLINE=0, ONLINE=1, BUSY=2, AWAY=3, SNOOZE=4, LF_TRADE, LF_PLAY, STATE_MAX, NOT_OFFLINE=8, ALL=9,
		ERR_NO_CLIENT=2, ERR_NO_CONNECTION=3,
	};
	static Steam* get_singleton();
	Steam();
	~Steam();

	CSteamID createSteamID(uint32 steamID, int accountType=-1);
	// Steamworks
	bool steamInit();
	bool isSteamRunning();
	// Apps
	bool hasOtherApp(int value);
	int getDLCCount();
	bool isDLCInstalled(int value);
	bool isSubscribedApp(int value);
	void requestAppProofOfPurchaseKey(int value);
	// Friends
	int getFriendCount();
	String getPersonaName();
	String getFriendPersonaName(int steam_id);
	void setGameInfo(const String& s_key, const String& s_value);
	void clearGameInfo();
	void inviteFriend(int id, const String& conString);
	void setPlayedWith(int steam_id);
	Array getRecentPlayers();
	// Users
//	void setServerInfo(const String& server_ip, int port); // Temporarily removed until fixed, broken in Godot 2.1.2
	int getSteamID();
	bool loggedOn();
	int getPlayerSteamLevel();
	// User Stats
	bool clearAchievement(const String& s_key);
	bool getAchievement(const String& s_key);
	float getStatFloat(const String& s_key);
	int getStatInt(const String& s_key);
	bool resetAllStats(bool bAchievementsToo=true);
	bool requestCurrentStats();
	bool setAchievement(const String& s_key);
	bool setStatFloat(const String& s_key, float value);
	bool setStatInt(const String& s_key, int value);
	bool storeStats();
	// Utils
	String getIPCountry();
	bool isOverlayEnabled();
	String getSteamUILanguage();
	int getAppID();

protected:
	static void _bind_methods();
	static Steam* singleton;
	void updateFriendList(int filter=NULL);

private:
	bool isInitSuccess;

	STEAM_CALLBACK(Steam, _server_connected, SteamServersConnected_t);
	STEAM_CALLBACK(Steam, _server_disconnected, SteamServersDisconnected_t);
	STEAM_CALLBACK(Steam, _join_requested, GameRichPresenceJoinRequested_t);

	void run_callbacks(){
		SteamAPI_RunCallbacks();
	}

	OBJ_TYPE(Steam, Object);
	OBJ_CATEGORY("Steam");
};
#endif // GODOTSTEAM_H
