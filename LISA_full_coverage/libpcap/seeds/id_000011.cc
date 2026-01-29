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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));

    const char *device = "lo";
    pcap_t *handle = pcap_create(device, errbuf);

    struct bpf_program prog;
    prog.bf_len = 0;
    prog.bf_insns = (struct bpf_insn *)0;

    struct pcap_pkthdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.caplen = 60;
    hdr.len = 60;

    u_char pkt[60];
    memset(pkt, 0xAA, sizeof(pkt));

    int rc_set_snaplen = pcap_set_snaplen(handle, 65535);
    int rc_set_promisc = pcap_set_promisc(handle, 1);
    int rc_set_timeout = pcap_set_timeout(handle, 1000);
    int rc_set_buffer = pcap_set_buffer_size(handle, 2 * 1024 * 1024);
    int rc_set_tsprec = pcap_set_tstamp_precision(handle, 1);

    int rc_activate = pcap_activate(handle);
    int rc_set_nonblock = pcap_setnonblock(handle, 1, errbuf);
    int rc_offline_filter = pcap_offline_filter(&prog, &hdr, pkt);
    pcap_perror(handle, "pcap_perror output");

    pcap_close(handle);

    (void)rc_set_snaplen;
    (void)rc_set_promisc;
    (void)rc_set_timeout;
    (void)rc_set_buffer;
    (void)rc_set_tsprec;
    (void)rc_activate;
    (void)rc_set_nonblock;
    (void)rc_offline_filter;

    return 66;
}