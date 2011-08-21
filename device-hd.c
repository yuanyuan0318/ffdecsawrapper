/*
 * Softcam plugin to VDR (C++)
 *
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * Or, point your browser to http://www.gnu.org/copyleft/gpl.html
 */

#include <stdlib.h>
#include <stdio.h>

#ifndef SASC

#include <vdr/channels.h>
#include <vdr/ci.h>
#include <vdr/dvbci.h>

#include "device.h"
#include "cam.h"
#include "scsetup.h"
#include "log-core.h"
#include "version.h"

SCAPIVERSTAG();

#if APIVERSNUM >= 10711
#ifdef WITH_HDDVB

// -- cScDvbSdFfDevice ---------------------------------------------------------

#include "../dvbhddevice/dvbhdffdevice.h"
#define SCDEVICE cScDvbHdFfDevice
#define DVBDEVICE cDvbHdFfDevice
#include "device-tmpl.c"
#undef SCDEVICE
#undef DVBDEVICE

// -- cScHdDevicePlugin --------------------------------------------------------

class cScHdDevicePlugin : public cScDevicePlugin {
public:
  virtual cDevice *Probe(int Adapter, int Frontend, uint32_t SubSystemId);
  virtual bool LateInit(cDevice *dev);
  virtual bool EarlyShutdown(cDevice *dev);
  };

static cScHdDevicePlugin _hddevplugin;

cDevice *cScHdDevicePlugin::Probe(int Adapter, int Frontend, uint32_t SubSystemId)
{
  static uint32_t SubsystemIds[] = {
    0x13C23009, // Technotrend S2-6400 HDFF development samples
    0x13C2300A, // Technotrend S2-6400 HDFF production version
    0x00000000
    };
  for(uint32_t *sid=SubsystemIds; *sid; sid++) {
    if(*sid==SubSystemId) {
      PRINTF(L_GEN_DEBUG,"creating HD-FF device %d/%d",Adapter,Frontend);
      return new cScDvbHdFfDevice(Adapter,Frontend,cScDevices::DvbOpen(DEV_DVB_CA,Adapter,Frontend,O_RDWR));
      }
    }
  return 0;
}

bool cScHdDevicePlugin::LateInit(cDevice *dev)
{
  cScDvbHdFfDevice *d=dynamic_cast<cScDvbHdFfDevice *>(dev);
  if(d) d->LateInit();
  return d!=0;
}

bool cScHdDevicePlugin::EarlyShutdown(cDevice *dev)
{
  cScDvbHdFfDevice *d=dynamic_cast<cScDvbHdFfDevice *>(dev);
  if(d) d->EarlyShutdown();
  return d!=0;
}

#endif //WITH_HDDVB
#endif //APIVERSNUM >= 10711
#endif //SASC