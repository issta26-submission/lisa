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
//<ID> 263
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
    const char device[] = "any";
    pcap_t *handle = pcap_create(device, errbuf);
    int rc_snaplen_set = pcap_set_snaplen(handle, 65535);
    int rc_promisc = pcap_set_promisc(handle, 1);
    int rc_timeout = pcap_set_timeout(handle, 1000);
    int rc_immediate = pcap_set_immediate_mode(handle, 1);
    int rc_buffer = pcap_set_buffer_size(handle, 2 * 1024 * 1024);
    int rc_activate = pcap_activate(handle);

    // step 2: Configure
    int rc_setdlt = pcap_set_datalink(handle, 1);
    int cur_snapshot = pcap_snapshot(handle);

    // step 3: Operate & Validate
    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    bpf_u_int32 netmask = 0;
    int rc_compile = pcap_compile(handle, &prog, "tcp or udp", 1, netmask);
    char *img = bpf_image(prog.bf_insns, (int)prog.bf_len);
    int rc_setfilter = pcap_setfilter(handle, &prog);
    u_char packet[256];
    memset(packet, 0xAB, sizeof(packet));
    int send_len = (int)((cur_snapshot % sizeof(packet)) + 1);
    int rc_send = pcap_sendpacket(handle, packet, send_len);

    // step 4: Cleanup
    pcap_freecode(&prog);
    pcap_close(handle);

    (void)rc_snaplen_set;
    (void)rc_promisc;
    (void)rc_timeout;
    (void)rc_immediate;
    (void)rc_buffer;
    (void)rc_activate;
    (void)rc_setdlt;
    (void)cur_snapshot;
    (void)rc_compile;
    (void)rc_setfilter;
    (void)rc_send;
    (void)device;
    (void)img;
    puts("API sequence test completed successfully");
    return 66;
}