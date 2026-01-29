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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    pcap_if_t *alldevs = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failed to find devices
        return -1;
    }
    if (alldevs == nullptr) {
        // no devices found
        return -2;
    }

    // pick the first device
    const char *dev = alldevs->name;
    if (dev == nullptr) {
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 2) Create a pcap handle for the device and configure it
    pcap_t *handle = pcap_create(dev, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -4;
    }

    // Set capture parameters: snapshot length, promiscuous mode, timeout
    if (pcap_set_snaplen(handle, 65535) != 0) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }
    if (pcap_set_promisc(handle, 1) != 0) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }
    if (pcap_set_timeout(handle, 1000) != 0) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -7;
    }

    // Activate the handle
    if (pcap_activate(handle) < 0) {
        // activation failed
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -8;
    }

    // 3) Optionally look up network and mask for this device to compile a filter
    bpf_u_int32 netp = 0, maskp = 0;
    if (pcap_lookupnet(dev, &netp, &maskp, errbuf) == -1) {
        // lookup failed, but we can continue with mask = 0
        maskp = 0;
    }

    // 4) Compile and set a simple filter (e.g., capture tcp packets)
    struct bpf_program fp;
    const char *filter_expr = "tcp";
    if (pcap_compile(handle, &fp, filter_expr, 1, maskp) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -9;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -10;
    }
    // free compiled filter resources
    pcap_freecode(&fp);

    // 5) Query some properties
    int dlt = pcap_datalink(handle);
    if (dlt < 0) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -11;
    }

    // 6) Try to capture a single packet (with the configured timeout)
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet_data = nullptr;
    int ret = pcap_next_ex(handle, &header, &packet_data);
    if (ret == -1) {
        // error while capturing
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -12;
    }
    // ret == 0 => timeout expired, ret == 1 => got a packet. Both are acceptable here.
    // We won't interpret the packet contents further in this test sequence.

    // 7) Clean up
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // If we reached here, the API sequence completed successfully
    return 66;
}