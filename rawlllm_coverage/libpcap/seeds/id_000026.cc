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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;

    // Find all available devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failure to enumerate devices
        return -1;
    }

    if (!alldevs) {
        // no device found
        return -1;
    }

    // Choose the first device from the list
    const char *dev = alldevs->name;

    // Try to obtain network and mask for the chosen device (non-fatal)
    bpf_u_int32 netp = 0, maskp = 0;
    if (pcap_lookupnet(dev, &netp, &maskp, errbuf) == -1) {
        // If lookup fails, continue with mask/net as 0
        netp = 0;
        maskp = 0;
    }

    // Open the device for live capture
    pcap_t *handle = pcap_open_live(dev, 65535, 0 /* non-promiscuous */, 1000 /* ms timeout */, errbuf);
    if (!handle) {
        pcap_freealldevs(alldevs);
        return -1;
    }

    // Compile a simple filter (capture IP packets)
    struct bpf_program fp;
    const char *filter_exp = "ip";
    if (pcap_compile(handle, &fp, filter_exp, 1 /* optimize */, maskp) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }

    // Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }

    // Cleanup compiled filter and handles
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Indicate success as requested
    return 66;
}