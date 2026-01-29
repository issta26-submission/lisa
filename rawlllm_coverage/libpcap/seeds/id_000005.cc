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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs = nullptr;
    pcap_t* handle = nullptr;
    struct bpf_program fp;
    bool fp_compiled = false;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        return -1;
    }
    if (!alldevs) {
        std::fprintf(stderr, "No devices found by pcap_findalldevs\n");
        return -2;
    }

    // Use the first device returned
    const char* devname = alldevs->name;
    if (!devname) {
        std::fprintf(stderr, "First device has no name\n");
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 2) Open the device for live capture
    // snaplen BUFSIZ, promisc = 1, timeout = 1000 ms
    handle = pcap_open_live(devname, BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        std::fprintf(stderr, "pcap_open_live failed for %s: %s\n", devname, errbuf);
        pcap_freealldevs(alldevs);
        return -4;
    }

    // 3) Optionally check data link type
    int dlt = pcap_datalink(handle);
    // dlt is an integer constant; we don't require a specific type here,
    // but we demonstrate the call. (No error return to check.)

    // 4) Compile a simple filter (e.g., capture only TCP packets)
    const char* filter_exp = "tcp";
    // Use PCAP_NETMASK_UNKNOWN for simplicity; on some platforms you may supply real netmask.
    if (pcap_compile(handle, &fp, filter_exp, 1, PCAP_NETMASK_UNKNOWN) == -1) {
        std::fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }
    fp_compiled = true;

    // 5) Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        std::fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        if (fp_compiled) pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // 6) Attempt to fetch one packet with pcap_next_ex (demonstrate usage)
    struct pcap_pkthdr* header = nullptr;
    const u_char* packet = nullptr;
    int res = pcap_next_ex(handle, &header, &packet);
    if (res == 1) {
        // Successfully got a packet; header and packet are valid.
        // We won't process the packet contents here; just demonstrate access.
        std::fprintf(stdout, "Captured one packet: len=%u\n", header->len);
    } else if (res == 0) {
        // Timeout elapsed
        std::fprintf(stdout, "pcap_next_ex: timeout (no packet captured within the timeout)\n");
    } else if (res == -1) {
        std::fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
        // Continue to cleanup and return error
        if (fp_compiled) pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -7;
    } else if (res == -2) {
        // EOF (e.g., when reading from a savefile)
        std::fprintf(stdout, "pcap_next_ex: EOF reached\n");
    }

    // 7) Clean up: free compiled filter, close handle, free device list
    if (fp_compiled) {
        pcap_freecode(&fp);
        fp_compiled = false;
    }
    pcap_close(handle);
    handle = nullptr;

    pcap_freealldevs(alldevs);
    alldevs = nullptr;

    // If we reached here, the sequence of libpcap API calls succeeded.
    return 66;
}