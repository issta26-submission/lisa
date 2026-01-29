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
//<ID> 286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));
    const char device[] = "any";
    pcap_t *handle = pcap_create(device, errbuf);

    int rc_snaplen = pcap_set_snaplen(handle, 65535);
    int rc_promisc = pcap_set_promisc(handle, 0);
    int rc_timeout = pcap_set_timeout(handle, 1000);
    int rc_tsprec = pcap_set_tstamp_precision(handle, 1);

    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    int rc_lookup = pcap_lookupnet(device, &net, &mask, errbuf);

    int rc_activate = pcap_activate(handle);

    /* Ensure non-blocking mode so subsequent pcap_next_ex does not hang */
    int rc_setnb = pcap_setnonblock(handle, 1, errbuf);

    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    int rc_compile = pcap_compile(handle, &prog, "tcp port 80", 1, mask);
    int rc_setfilter = pcap_setfilter(handle, &prog);

    struct pcap_pkthdr *hdr;
    const u_char *pkt_data;
    int rc_next = pcap_next_ex(handle, &hdr, &pkt_data);

    int tval = pcap_tstamp_type_name_to_val("micro");
    const char *tname = pcap_tstamp_type_val_to_name(tval);
    (void)tname;

    pcap_freecode(&prog);
    pcap_close(handle);

    (void)rc_snaplen;
    (void)rc_promisc;
    (void)rc_timeout;
    (void)rc_tsprec;
    (void)rc_lookup;
    (void)rc_activate;
    (void)rc_setnb;
    (void)rc_compile;
    (void)rc_setfilter;
    (void)rc_next;
    (void)hdr;
    (void)pkt_data;
    (void)net;
    (void)mask;
    (void)device;
    (void)errbuf;

    puts("API sequence test completed successfully");
    return 66;
}