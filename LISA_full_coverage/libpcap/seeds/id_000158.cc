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
//<ID> 158
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
    pcap_if_t *alldevs = NULL;
    pcap_findalldevs(&alldevs, errbuf);
    const char *device = alldevs->name;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    pcap_lookupnet(device, &net, &mask, errbuf);

    // step 2: Configure
    pcap_t *handle = pcap_create(device, errbuf);
    pcap_set_snaplen(handle, 65535);
    pcap_set_promisc(handle, 1);
    pcap_set_timeout(handle, 1000);
    pcap_set_immediate_mode(handle, 1);
    pcap_activate(handle);

    // step 3: Operate / Validate
    int *dlt_list = NULL;
    pcap_list_datalinks(handle, &dlt_list);
    int chosen_dlt = dlt_list[0];
    pcap_set_datalink(handle, chosen_dlt);
    struct bpf_program prog;
    pcap_compile(handle, &prog, "tcp", 1, net);
    pcap_setfilter(handle, &prog);
    pcap_dumper_t *dumper = pcap_dump_open(handle, "capture_out.pcap");
    pcap_freecode(&prog);

    // step 4: Cleanup
    pcap_dump_close(dumper);
    pcap_free_datalinks(dlt_list);
    pcap_freealldevs(alldevs);
    pcap_close(handle);

    (void)errbuf;
    (void)device;
    (void)net;
    (void)mask;
    (void)chosen_dlt;

    puts("API sequence test completed successfully");
    return 66;
}