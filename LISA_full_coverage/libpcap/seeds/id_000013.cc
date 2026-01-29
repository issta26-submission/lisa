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
//<ID> 13
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
    struct bpf_program filter_prog;
    memset(&filter_prog, 0, sizeof(filter_prog));
    struct pcap_pkthdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    u_char packet[64];
    memset(packet, 0, sizeof(packet));

    pcap_set_snaplen(handle, 65535);
    pcap_set_promisc(handle, 1);
    pcap_set_timeout(handle, 100);
    pcap_set_buffer_size(handle, 1024 * 1024);
    pcap_setnonblock(handle, 1, errbuf);

    pcap_activate(handle);
    pcap_compile(handle, &filter_prog, "tcp", 1, (bpf_u_int32)0);
    hdr.ts.tv_sec = 0;
    hdr.ts.tv_usec = 0;
    hdr.caplen = sizeof(packet);
    hdr.len = sizeof(packet);
    pcap_offline_filter(&filter_prog, &hdr, packet);
    pcap_perror(handle, "pcap_perror after offline filter");

    pcap_freecode(&filter_prog);
    pcap_close(handle);

    return 66;
}