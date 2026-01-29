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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // Include libpcap header here to make the function self-contained for compilation.
    // (Preprocessor directives are valid anywhere in the source file.)
    #include <pcap/pcap.h>
    #include <cstring>

    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));

    pcap_if_t *alldevs = nullptr;
    // Discover devices (optional; not fatal if it fails)
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // discovery failed: errbuf contains error text; continue using dead handle for compilation test
        // We won't return immediately; we'll still exercise other APIs.
    }

    // If we found at least one device, try to open it live (non-promiscuous, 1s timeout).
    pcap_t *live_handle = nullptr;
    const char *first_dev_name = nullptr;
    if (alldevs != nullptr) {
        first_dev_name = alldevs->name;
        if (first_dev_name != nullptr) {
            live_handle = pcap_open_live(first_dev_name, 65535, 0 /*not promiscuous*/, 1000 /*ms timeout*/, errbuf);
            // pcap_open_live returns nullptr on failure; that's non-fatal for this test sequence.
        }
    }

    // If we have a live handle, query link-layer header type and attempt a non-blocking mode set and a single read attempt.
    if (live_handle != nullptr) {
        // Get data-link type
        int dlt = pcap_datalink(live_handle);
        (void)dlt; // use dlt (silence unused variable if not printed)

        // Try to set non-blocking mode (may fail depending on platform/permissions)
        // pcap_setnonblock returns 0 on success, -1 on failure (and fills errbuf)
        int nb_ret = pcap_setnonblock(live_handle, 1, errbuf);
        (void)nb_ret;

        // Try a single (non-blocking) read with pcap_next_ex. This is allowed to time out or fail.
        struct pcap_pkthdr *header = nullptr;
        const u_char *pkt_data = nullptr;
        int nex = pcap_next_ex(live_handle, &header, &pkt_data);
        // nex == 1: a packet was read
        // nex == 0: timeout expired
        // nex == -1 or -2: error or no more packets; we ignore for this test

        // Close the live handle after usage
        pcap_close(live_handle);
        live_handle = nullptr;
    }

    // Use an "open_dead" pcap_t to demonstrate BPF compilation without needing an actual interface.
    pcap_t *dead = pcap_open_dead(DLT_EN10MB, 65535); // Ethernet DLT, common choice
    if (dead == nullptr) {
        // Couldn't create dead handle; clean up and fail
        if (alldevs != nullptr) pcap_freealldevs(alldevs);
        return -1;
    }

    // Compile a BPF filter using the dead handle (no live capture required)
    struct bpf_program fp;
    const char *filter_exp = "tcp port 80";
    // PCAP_NETMASK_UNKNOWN is used because we don't have a real netmask here
    if (pcap_compile(dead, &fp, filter_exp, 1 /*optimize*/, PCAP_NETMASK_UNKNOWN) != 0) {
        // compilation failed; clean up
        pcap_close(dead);
        if (alldevs != nullptr) pcap_freealldevs(alldevs);
        return -2;
    }

    // At this point the filter was compiled successfully. Free the compiled filter.
    pcap_freecode(&fp);

    // Close the dead handle
    pcap_close(dead);

    // Free device list if it was allocated
    if (alldevs != nullptr) pcap_freealldevs(alldevs);

    // If we reached this point, the sequence of libpcap API usages succeeded.
    return 66;
}