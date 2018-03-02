#pragma once
#include <Platform.h>

bool RELEASE_BUILD = false;
bool USE_NETWORK_SAVES = false;
bool USE_CHEATS = false;
bool USE_KONGREGATE = false;
bool USE_GOOGLE_ANALYTICS = false;
bool USE_REMOTE_TELEMETRY = false;
bool USE_DEBUG_PANE = false;

const int32 PUBLISH_LOCAL_DEBUG = 0;
const int32 PUBLISH_DAJ_WEB = 1;
const int32 PUBLISH_KONGREGATE = 2;
const int32 PUBLISH_MOCHI = 3;

int32 CONFIGURATION = PUBLISH_DAJ_WEB;

void Config();