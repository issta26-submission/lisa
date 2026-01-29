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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // This function exercises a sequence of common libpcap APIs.
    // It returns 66 on success; any early failure returns a negative code.
    pcap_if_t *alldevs = nullptr;
    pcap_t *handle = nullptr;
    struct bpf_program fp{};
    bool compiled = false;
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *devname = nullptr;

    // Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) != 0) {
        // Failure enumerating devices
        return -1;
    }

    // Choose the first device if available; otherwise fall back to lookupdev
    if (alldevs != nullptr) {
        devname = alldevs->name;
    } else {
        // pcap_lookupdev is deprecated on some platforms but available on many
        char *lookup = pcap_lookupdev(errbuf);
        if (lookup == nullptr) {
            // No device found
            return -2;
        }
        devname = lookup;
    }

    // Open the device for live capture
    // snapshot length: 65536, promiscuous: 1, read timeout: 1000 ms
    handle = pcap_open_live(devname, 65536, 1, 1000, errbuf);
    if (handle == nullptr) {
        if (alldevs) pcap_freealldevs(alldevs);
        return -3;
    }

    // Compile a simple BPF filter (e.g., capture TCP packets)
    const char filter_exp[] = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1 /* optimize */, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -4;
    }
    compiled = true;

    // Apply the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -5;
    }

    // Free the compiled program since it's been set
    pcap_freecode(&fp);
    compiled = false;

    // Set non-blocking mode
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        // Non-fatal for this sequence: continue but note the failure by closing and returning error
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -6;
    }

    // Verify non-blocking mode
    int nb = pcap_getnonblock(handle, errbuf);
    if (nb == -1) {
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -7;
    }

    // Try to capture a single packet (non-blocking: may return 0 for timeout)
    struct pcap_pkthdr *header = nullptr;
    const u_char *pkt_data = nullptr;
    int res = pcap_next_ex(handle, &header, &pkt_data);
    if (res == -1) {
        // Error during capture
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -8;
    }
    // res == 1 -> packet read, res == 0 -> timeout (non-blocking), res == -2 -> EOF (offline)
    // For this test sequence, either timeout or a packet is acceptable.

    // Retrieve statistics (may fail on some platforms)
    struct pcap_stat stats{};
    (void) pcap_stats(handle, &stats); // ignore return value for this test

    // Optionally show datalink type / values (no output per instructions)
    (void) pcap_datalink(handle);

    // Clean up
    pcap_close(handle);
    if (alldevs) pcap_freealldevs(alldevs);

    // Success
    return 66;
}