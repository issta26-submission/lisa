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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // Include headers here to keep this function self-contained when copied into a file.
    // (Including inside a function is allowed; preprocessors run before compilation.)
    #include <pcap/pcap.h>
    #include <cstring>

    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));

    pcap_if_t *alldevs = nullptr;
    pcap_t *handle = nullptr;
    struct bpf_program fp;
    bool compiled = false;
    const char *device = nullptr;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // If findalldevs fails, try the deprecated lookup as a fallback
        const char *lookup = pcap_lookupdev(errbuf);
        if (!lookup) {
            // Could not find any device
            return -1;
        }
        device = lookup;
    } else {
        if (alldevs == nullptr) {
            // No devices found
            return -2;
        }
        // Choose the first device in the list
        device = alldevs->name;
    }

    if (!device) {
        if (alldevs) pcap_freealldevs(alldevs);
        return -3;
    }

    // 2) Open the device for live capture
    // snaplen 65535, promiscuous mode, timeout 1000 ms
    handle = pcap_open_live(device, 65535, 1, 1000, errbuf);
    if (!handle) {
        if (alldevs) pcap_freealldevs(alldevs);
        return -4;
    }

    // 3) Query some properties
    int dlt = pcap_datalink(handle);
    (void)dlt; // avoid unused variable warning if not used further
    int snaplen = pcap_snapshot(handle);
    (void)snaplen;

    // 4) Compile and set a simple BPF filter (e.g., capture only TCP)
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -5;
    }
    compiled = true;

    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -6;
    }

    // 5) Try to read a single packet (pcap_next_ex returns 1=packet, 0=timeout, -1=error, -2=EOF)
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet = nullptr;
    int res = pcap_next_ex(handle, &header, &packet);
    if (res == -1) {
        // read error
        pcap_freecode(&fp);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -7;
    }
    // res can be 0 (timeout) or 1 (got packet) or -2 (no more packets from savefile).
    // For this API sequence test, we treat timeout as non-fatal (still a correct usage).
    // If needed, inspect header/packet when res == 1:
    if (res == 1 && header && packet) {
        // Example usage: access lengths (no modification)
        (void)header->len;
        (void)header->caplen;
        (void)packet;
    }

    // 6) Clean up resources
    if (compiled) {
        pcap_freecode(&fp);
    }
    pcap_close(handle);
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }

    // If we reached this point, the API sequence succeeded
    return 66;
}