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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // include here to make the function self-contained in a single snippet
    #include <pcap/pcap.h>

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;

    // Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        return -1;
    }
    if (alldevs == nullptr) {
        // no devices found
        return -1;
    }

    // Pick the first device
    pcap_if_t *dev = alldevs;

    // Try to look up network and mask (non-fatal if it fails)
    bpf_u_int32 netp = 0, maskp = 0;
    if (pcap_lookupnet(dev->name, &netp, &maskp, errbuf) == -1) {
        netp = 0;
        maskp = 0;
    }

    // Open the device for live capture
    int snaplen = 65535;      // max bytes per packet to capture
    int promisc = 0;         // non-promiscuous to avoid requiring root in some environments
    int to_ms = 1000;        // read timeout in milliseconds
    pcap_t *handle = pcap_open_live(dev->name, snaplen, promisc, to_ms, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -1;
    }

    // Compile and set a simple filter
    struct bpf_program fcode;
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fcode, filter_exp, 1 /* optimize */, netp) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }
    if (pcap_setfilter(handle, &fcode) == -1) {
        pcap_freecode(&fcode);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }

    // Check datalink type (demonstration of another API)
    int dlt = pcap_datalink(handle);
    (void)dlt; // silence unused-variable warnings

    // Attempt to read a single packet (may timeout; treat timeout as non-fatal)
    struct pcap_pkthdr *header = nullptr;
    const u_char *pkt_data = nullptr;
    int rc = pcap_next_ex(handle, &header, &pkt_data);
    if (rc == -1) {
        // fatal error while reading
        pcap_freecode(&fcode);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }
    // rc == 0 => timeout, rc == 1 => packet read, rc == -2 => file fully read (not expected for live capture)
    (void)header;
    (void)pkt_data;

    // Clean up
    pcap_freecode(&fcode);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success: return the required value
    return 66;
}