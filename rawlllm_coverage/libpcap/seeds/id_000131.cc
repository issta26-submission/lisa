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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    pcap_if_t *alldevs = nullptr;
    pcap_t *handle = nullptr;
    struct bpf_program fp;
    bool compiled = false;
    char errbuf[PCAP_ERRBUF_SIZE];

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // pcap_findalldevs failed; errbuf contains a human-readable error
        return 1;
    }

    // Ensure we have at least one device
    if (alldevs == nullptr) {
        // no devices found
        return 2;
    }

    // 2) Open the first device for live capture
    const char *devname = alldevs->name;
    if (devname == nullptr) {
        pcap_freealldevs(alldevs);
        return 3;
    }

    // snaplen 65535, promiscuous mode, timeout 1000ms
    handle = pcap_open_live(devname, 65535, 1, 1000, errbuf);
    if (handle == nullptr) {
        // failed to open device
        pcap_freealldevs(alldevs);
        return 4;
    }

    // 3) Query snapshot length and data link type (demonstrates additional API usage)
    int snaplen = pcap_snapshot(handle);
    int dlt = pcap_datalink(handle);
    // Avoid unused-variable warnings
    (void)snaplen;
    (void)dlt;

    // 4) Compile a simple filter (capture only TCP packets)
    if (pcap_compile(handle, &fp, "tcp", 1, PCAP_NETMASK_UNKNOWN) == -1) {
        // compilation failed
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 5;
    }
    compiled = true;

    // 5) Apply the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        // setfilter failed
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 6;
    }

    // 6) Attempt to read a single packet using pcap_next_ex
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet = nullptr;
    int res = pcap_next_ex(handle, &header, &packet);
    if (res == 1) {
        // Packet read successfully. Do minimal validation/use of the packet to show usage.
        if (header != nullptr && packet != nullptr && header->len > 0) {
            volatile u_char b = packet[0];
            (void)b; // use the byte to avoid optimizing out
        }
    } else if (res == 0) {
        // Timeout expired, no packet read during the interval. This is acceptable for a test.
    } else if (res == -1) {
        // Error while reading packet; fall through to cleanup and return a non-success code.
        if (compiled) pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 7;
    } else if (res == -2) {
        // EOF was reached in offline capture; treat as non-fatal for this test
    }

    // 7) Clean up resources
    if (compiled) pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success
    return 66;
}