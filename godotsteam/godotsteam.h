#ifndef GODOTSTEAM_H
#define GODOTSTEAM_H

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

	// Steamworks
	bool steamInit();
	bool isSteamRunning();
	// Apps
	bool hasOtherApp(int value);
	int getDLCCount();
	bool isDLCInstalled(int value);
	void requestAppProofOfPurchaseKey(int value);
	// Friends
	int getFriendCount();
	String getPersonaName();
	// Users
	int getSteamID();
	bool loggedOn();
	int getPlayerSteamLevel();
	// User Stats
	bool clearAchievement(const String& s_key);
	bool getAchievement(const String& s_key);
	float getStatFloat(const String& s_key);
	int getStatInt(const String& s_key);
	bool resetAllStats(bool bAchievementsToo);
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

private:
	bool isInitSuccess;
	OBJ_TYPE(Steam, Object);
	OBJ_CATEGORY("Steam");
};
#endif // GODOTSTEAM_H