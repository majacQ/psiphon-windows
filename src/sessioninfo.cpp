/*
 * Copyright (c) 2011, Psiphon Inc.
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"
#include "sessioninfo.h"
#include "psiclient.h"
#include <sstream>

void SessionInfo::Set(const ServerEntry& serverEntry)
{
    m_serverEntry = serverEntry;
}

bool SessionInfo::ParseHandshakeResponse(const string& response)
{
    // Expected response:
    //
    // Upgrade: <url> \n        (zero or one)
    // PSK: <hexstring>\n       (zero or one)
    // HomePage: <url>\n        (zero or more)
    // Server: <hexstring>\n    (zero or more)
    // SSHPort: <string>\n         (zero or one)
    // SSHUsername: <string>\n  (zero or one)
    // SSHPassword: <string>\n  (zero or one)
    // SSHHostKey: <string>\n   (zero or one)

    static const char* UPGRADE_PREFIX = "Upgrade: ";
    static const char* PSK_PREFIX = "PSK: ";
    static const char* SSH_PORT_PREFIX = "SSHPort: ";
    static const char* SSH_USERNAME_PREFIX = "SSHUsername: ";
    static const char* SSH_PASSWORD_PREFIX = "SSHPassword: ";
    static const char* SSH_HOST_KEY_PREFIX = "SSHHostkey: ";
    static const char* HOMEPAGE_PREFIX = "Homepage: ";
    static const char* SERVER_PREFIX = "Server: ";

    m_upgradeVersion.clear();
    m_psk.clear();
    m_sshPort.clear();
    m_sshUsername.clear();
    m_sshPassword.clear();
    m_sshHostKey.clear();
    m_homepages.clear();
    m_servers.clear();

    stringstream stream(response);
    string item;

    while (getline(stream, item, '\n'))
    {
        if (0 == item.find(UPGRADE_PREFIX))
        {
            item.erase(0, strlen(UPGRADE_PREFIX));
            m_upgradeVersion = item;
        }
        else if (0 == item.find(PSK_PREFIX))
        {
            item.erase(0, strlen(PSK_PREFIX));
            m_psk = item;
        }
        else if (0 == item.find(SSH_PORT_PREFIX))
        {
            item.erase(0, strlen(SSH_PORT_PREFIX));
            m_sshPort = item;
        }
        else if (0 == item.find(SSH_USERNAME_PREFIX))
        {
            item.erase(0, strlen(SSH_USERNAME_PREFIX));
            m_sshUsername = item;
        }
        else if (0 == item.find(SSH_PASSWORD_PREFIX))
        {
            item.erase(0, strlen(SSH_PASSWORD_PREFIX));
            m_sshPassword = item;
        }
        else if (0 == item.find(SSH_HOST_KEY_PREFIX))
        {
            item.erase(0, strlen(SSH_HOST_KEY_PREFIX));
            m_sshHostKey = item;
        }
        else if (0 == item.find(HOMEPAGE_PREFIX))
        {
            item.erase(0, strlen(HOMEPAGE_PREFIX));
            m_homepages.push_back(item);
        }
        else if  (0 == item.find(SERVER_PREFIX))
        {
            item.erase(0, strlen(SERVER_PREFIX));
            m_servers.push_back(item);
        }
    }

    // TODO: more explicit validation?  Eg, got exactly one non-blank PSK

    return true;
}
