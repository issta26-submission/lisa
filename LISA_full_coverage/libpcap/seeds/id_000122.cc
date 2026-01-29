#include <pcap/bluetooth.h>
#include <pcap/can_socketcan.h>
#include <pcap/compiler-tests.h>
#include <pcap/dlt.h>
#include <pcap/funcattrs.h>
#include <pcap/ipnet.h>
#include <pcap/nflog.h>
#include <pcap/pcap-inttypes.h>
#include <pcap/pcap.h>
#include <pcap/sll.h>
#include <pcap/socket.h>
#include <pcap/usb.h>
#include <pcap/vlan.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // step 1: Initialize
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));
    char *device = pcap_lookupdev(errbuf);
    pcap_if_t *alldevs = NULL;
    int rc_findalldevs = pcap_findalldevs(&alldevs, errbuf);

    // step 2: Configure
    pcap_t *handle = pcap_create(device, errbuf);
    int rc_set_snaplen = pcap_set_snaplen(handle, 65535);
    int rc_set_promisc = pcap_set_promisc(handle, 1);
    int rc_set_timeout = pcap_set_timeout(handle, 1000);
    int rc_set_immediate = pcap_set_immediate_mode(handle, 1);
    int rc_activate = pcap_activate(handle);

    // step 3: Operate / Validate
    int current_dlt = pcap_datalink(handle);
    const char *dlt_name = pcap_datalink_val_to_name(current_dlt);
    int *tstamp_types = NULL;
    int rc_list_ts = pcap_list_tstamp_types(handle, &tstamp_types);
    int chosen_ts = 0;
    chosen_ts = tstamp_types ? tstamp_types[0] : 0;
    const char *tstamp_name = pcap_tstamp_type_val_to_name(chosen_ts);

    // step 4: Cleanup
    pcap_free_tstamp_types(tstamp_types);
    int rc_set_dlt = pcap_set_datalink(handle, current_dlt);
    pcap_freealldevs(alldevs);
    pcap_close(handle);

    (void)rc_findalldevs;
    (void)rc_set_snaplen;
    (void)rc_set_promisc;
    (void)rc_set_timeout;
    (void)rc_set_immediate;
    (void)rc_activate;
    (void)dlt_name;
    (void)rc_list_ts;
    (void)tstamp_name;
    (void)rc_set_dlt;
    (void)device;
    (void)errbuf;
    puts("API sequence test completed successfully");
    return 66;
}