/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <android-base/properties.h>
#include "vendor_init.h"
#include "property_service.h"
#include <sys/sysinfo.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;
using android::init::property_set;

int is2GB()
{
    struct sysinfo sys;
    sysinfo(&sys);
    return sys.totalram > 1024ull * 1024 * 1024;
}

void vendor_load_properties()
{
    bool MSIM;

    std::string platform = GetProperty("ro.board.platform","");
    if (platform != ANDROID_TARGET)
        return;

    MSIM = false;
    std::string radio = GetProperty("ro.boot.radio","");
    std::string sku = GetProperty("ro.boot.hardware.sku","");
    std::string carrier = GetProperty("ro.boot.carrier","");

    if (is2GB()) {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "192m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "512k");
        property_set("dalvik.vm.heapmaxfree", "8m");
    } else {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "96m");
        property_set("dalvik.vm.heapsize", "256m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "2m");
        property_set("dalvik.vm.heapmaxfree", "8m");
    }
    property_set("ro.gsm.data_retry_config", "default_randomization=2000,max_retries=infinite,1000,1000,80000,125000,485000,905000");
	
    if (sku == "XT1540") {
        property_set("ro.gsm.data_retry_config", "");
    } else if (sku == "XT1541") {
        property_set("ro.fsg-id", "emea");
        property_set("persist.radio.process_sups_ind", "0");
    } else if (sku == "XT1542") {
        property_set("persist.radio.all_bc_msg", "all");
        property_set("persist.radio.process_sups_ind", "1");
    } else if (sku == "XT1543" || radio == "0x6") {
        MSIM = true;
        property_set("persist.radio.all_bc_msg", "all");
        property_set("persist.radio.process_sups_ind", "1");
    } else if (sku == "XT1544") {
        MSIM = true;
        property_set("persist.radio.all_bc_msg", "all");
        property_set("persist.radio.process_sups_ind", "1");
    } else if (sku == "XT1548") {
        if (carrier == "sprint") {
            property_set("ro.cdma.home.operator.alpha", "Sprint");
            property_set("ro.cdma.home.operator.numeric", "310120");
            property_set("ro.fsg-id", "sprint");
        } else {
            property_set("ro.cdma.home.operator.alpha", "U.S. Cellular");
            property_set("ro.cdma.home.operator.numeric", "311580");
            property_set("ro.fsg-id", "usc");
        }
        property_set("ro.cdma.data_retry_config", "max_retries=infinite,0,0,10000,10000,100000,10000,10000,10000,10000,140000,540000,960000");
        property_set("ro.product.locale.region", "US");
        property_set("gsm.sim.operator.iso-country", "US");
        property_set("gsm.operator.iso-country", "US");
        property_set("ril.subscription.types", "NV,RUIM");
        property_set("ro.telephony.default_cdma_sub", "0");
        property_set("ro.telephony.default_network", "8");
        property_set("ro.telephony.get_imsi_from_sim", "true");
        property_set("telephony.lteOnCdmaDevice", "1");
    } else if (sku == "XT1550") {
        MSIM = true;
        property_set("ro.fsg-id", "apac");
        property_set("persist.radio.process_sups_ind", "0");
    }
    if (GetProperty("ro.telephony.default_network", "").empty()) {
        property_set("ro.telephony.default_network", "9");
    }
    if (MSIM) {
        property_set("persist.radio.force_get_pref", "1");
        property_set("persist.radio.multisim.config", "dsds");
        property_set("ro.telephony.ril.config", "simactivation");
    }
}
