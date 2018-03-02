#include "../../Public/main/BuildSettings.h"

void Config()
{
	switch (CONFIGURATION)
	{
	case PUBLISH_MOCHI:
	{
		RELEASE_BUILD = false;
		USE_CHEATS = false;
		USE_KONGREGATE = false;
		USE_GOOGLE_ANALYTICS = false;
		USE_REMOTE_TELEMETRY = false;
		break;
	}
	case PUBLISH_LOCAL_DEBUG:
	{
		RELEASE_BUILD = false;
		USE_CHEATS = true;
		USE_KONGREGATE = false;
		USE_GOOGLE_ANALYTICS = false;
		USE_REMOTE_TELEMETRY = false;
		break;
	}
	case PUBLISH_DAJ_WEB:
	{
		RELEASE_BUILD = true;
		USE_KONGREGATE = false;
		USE_GOOGLE_ANALYTICS = true;
		USE_REMOTE_TELEMETRY = false;
		USE_DEBUG_PANE = false;
		break;
	}
	case PUBLISH_KONGREGATE:
	{
		RELEASE_BUILD = true;
		USE_KONGREGATE = true;
		USE_GOOGLE_ANALYTICS = true;
		USE_REMOTE_TELEMETRY = true;
		USE_DEBUG_PANE = false;
		break;
	}
	default:
	{
		break;
	}
	}
}
