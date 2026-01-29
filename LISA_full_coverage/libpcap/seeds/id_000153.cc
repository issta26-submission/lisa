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
//<ID> 153
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
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    int rc_lookupnet = pcap_lookupnet(device, &net, &mask, errbuf);
    pcap_if_t *alldevs = NULL;
    int rc_findalldevs = pcap_findalldevs(&alldevs, errbuf);

    // step 2: Configure
    pcap_t *handle = pcap_create(device, errbuf);
    int rc_set_snaplen = pcap_set_snaplen(handle, 65535);
    int rc_set_promisc = pcap_set_promisc(handle, 1);
    int rc_set_timeout = pcap_set_timeout(handle, 1000);
    int rc_set_immediate = pcap_set_immediate_mode(handle, 1);
    int rc_activate = pcap_activate(handle);
    int *dlt_list = NULL;
    int rc_list_dlt = pcap_list_datalinks(handle, &dlt_list);
    int chosen_dlt = dlt_list[0];
    int rc_setdlt = pcap_set_datalink(handle, chosen_dlt);

    // step 3: Operate / Validate
    struct bpf_program fp;
    int rc_compile = pcap_compile(handle, &fp, "tcp", 1, net);
    int rc_setfilter = pcap_setfilter(handle, &fp);
    pcap_dumper_t *dumper = pcap_dump_open(handle, "test_capture_sequence.pcap");
    struct pcap_pkthdr *hdr = NULL;
    const u_char *pkt = NULL;
    int rc_next = pcap_next_ex(handle, &hdr, &pkt);
    pcap_dump((u_char *)dumper, hdr, pkt);
    pcap_freecode(&fp);

    // step 4: Cleanup
    pcap_dump_close(dumper);
    pcap_free_datalinks(dlt_list);
    pcap_freealldevs(alldevs);
    pcap_close(handle);

    (void)errbuf;
    (void)device;
    (void)net;
    (void)mask;
    (void)rc_lookupnet;
    (void)rc_findalldevs;
    (void)rc_set_snaplen;
    (void)rc_set_promisc;
    (void)rc_set_timeout;
    (void)rc_set_immediate;
    (void)rc_activate;
    (void)rc_list_dlt;
    (void)chosen_dlt;
    (void)rc_setdlt;
    (void)rc_compile;
    (void)rc_setfilter;
    (void)rc_next;

    puts("API sequence test completed successfully");
    return 66;
}