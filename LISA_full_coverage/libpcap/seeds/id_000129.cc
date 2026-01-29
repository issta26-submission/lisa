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
//<ID> 129
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
    const char *libver = pcap_lib_version();

    // step 2: Configure
    pcap_t *handle = pcap_create(device, errbuf);
    int rc_set_snaplen = pcap_set_snaplen(handle, 65535);
    int rc_set_promisc = pcap_set_promisc(handle, 1);
    int rc_set_timeout = pcap_set_timeout(handle, 1000);
    int rc_activate = pcap_activate(handle);

    // step 3: Operate / Validate
    int *dlt_list = NULL;
    int rc_list_dlt = pcap_list_datalinks(handle, &dlt_list);
    int chosen_dlt = dlt_list[0];
    const char *dlt_name = pcap_datalink_val_to_name(chosen_dlt);
    int rc_setdlt = pcap_set_datalink(handle, chosen_dlt);
    int *tstamp_types = NULL;
    int rc_list_tst = pcap_list_tstamp_types(handle, &tstamp_types);
    int chosen_tst = tstamp_types[0];
    const char *tst_name = pcap_tstamp_type_val_to_name(chosen_tst);

    // step 4: Cleanup
    pcap_free_datalinks(dlt_list);
    pcap_free_tstamp_types(tstamp_types);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    (void)errbuf;
    (void)device;
    (void)rc_findalldevs;
    (void)libver;
    (void)rc_set_snaplen;
    (void)rc_set_promisc;
    (void)rc_set_timeout;
    (void)rc_activate;
    (void)rc_list_dlt;
    (void)chosen_dlt;
    (void)dlt_name;
    (void)rc_setdlt;
    (void)rc_list_tst;
    (void)chosen_tst;
    (void)tst_name;

    puts("API sequence test completed successfully");
    return 66;
}