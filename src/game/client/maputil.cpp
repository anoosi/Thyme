/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Map file handling utility functions.
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "maputil.h"
#include "globaldata.h"
#include "staticnamekey.h"
#include <algorithm>

#ifndef GAME_DLL
WaypointMap *g_waypoints;
MapCache *g_theMapCache;
#endif

const char *const MapCache::s_mapDirName = "Maps";
const char *const MapCache::s_mapExtension = "map";
const char *const MapCache::s_mapCacheName = "MapCache.ini";

// Updates a waypoint map from the global waypoints map with initial cam position
// and player start waypoints.
void WaypointMap::Update()
{
    if (g_waypoints != nullptr) {
        clear();
        Utf8String key_name = g_theNameKeyGenerator->Key_To_Name(g_theInitialCameraPositionKey);
        auto it = g_waypoints->find(key_name);

        if (it != g_waypoints->end()) {
            (*this)[key_name] = it->second;
        }

        m_numStartSpots = 0;

        for (int i = 1; i <= 8; ++i) {
            key_name.Format("Player_%d_Start", i);
            it = g_waypoints->find(key_name);

            if (it != g_waypoints->end()) {
                break;
            }

            (*this)[key_name] = it->second;
            ++m_numStartSpots;
        }

        m_numStartSpots = std::max(m_numStartSpots, 1);
    } else {
        m_numStartSpots = 1;
    }
}

Utf8String MapCache::Get_User_Map_Dir()
{
    Utf8String dir = g_theWriteableGlobalData->m_userDataDirectory;
    dir += Get_Map_Dir();

    return dir;
}

void MapCache::Update_Cache()
{
#ifdef GAME_DLL
    Call_Method<void, MapCache>(PICK_ADDRESS(0x0047F3B0, 0x009EE29A), this);
#endif
}

MapMetaData *MapCache::Find_Map(Utf8String map_name)
{
    map_name.To_Lower();
    auto it = find(map_name);

    if (it == end()) {
        return nullptr;
    } else {
        return &it->second;
    }
}

void Find_Draw_Positions(int start_x, int start_y, int width, int height, Region3D extent, ICoord2D *ul, ICoord2D *lr)
{
    float max_w = extent.Width() / (width * 1.0f);
    float max_h = extent.Height() / (height * 1.0f);

    if (max_w < max_h) {
        float fitted_w = extent.Width() / max_h;
        float fitted_h = extent.Height() / max_h;
        ul->x = (width - fitted_w) / 2.0f;
        ul->y = 0;
        lr->x = width - ul->x;
        lr->y = fitted_h;
    } else {
        float fitted_w = extent.Width() / max_w;
        float fitted_h = extent.Height() / max_w;
        ul->x = 0;
        ul->y = (height - fitted_h) / 2.0f;
        lr->x = fitted_w;
        lr->y = height - ul->y;
    }

    ul->x += start_x;
    ul->y += start_y;
    lr->x += start_x;
    lr->y += start_y;
}
