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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));

    // 1) Get libpcap version string (informational)
    const char *version = pcap_lib_version();
    if (version) {
        // not required, but show it's retrieved (no I/O required; using cerr for diagnostics)
        std::cerr << "pcap version: " << version << std::endl;
    }

    // 2) Find all devices
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "pcap_findalldevs failed: " << errbuf << std::endl;
        return -1;
    }

    // 3) Pick a device name (first available). If none, try the special "any" device (Linux)
    std::string dev;
    if (alldevs != nullptr) {
        pcap_if_t *d = alldevs;
        // pick the first device with a name
        while (d && (!d->name || std::strlen(d->name) == 0)) d = d->next;
        if (d && d->name) dev = d->name;
    }
    if (dev.empty()) {
        dev = "any"; // fallback (may not exist on non-Linux systems)
    }

    // 4) Open the device for live capture
    // snapshot length 65535, non-promiscuous (0), timeout 1000ms
    pcap_t *handle = pcap_open_live(dev.c_str(), 65535, 0, 1000, errbuf);
    if (!handle) {
        std::cerr << "pcap_open_live failed for device '" << dev << "': " << errbuf << std::endl;
        if (alldevs) pcap_freealldevs(alldevs);
        return -2;
    }

    // 5) Query datalink type
    int dlt = pcap_datalink(handle);
    std::cerr << "datalink on device '" << dev << "': " << dlt << std::endl;

    // 6) Compile a BPF filter ("tcp")
    struct bpf_program fp;
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "pcap_compile failed: " << pcap_geterr(handle) << std::endl;
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -3;
    }

    // 7) Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        std::cerr << "pcap_setfilter failed: " << pcap_geterr(handle) << std::endl;
        pcap_freecode(&fp);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -4;
    }

    // 8) Try to capture up to one packet (with a few retries for timeouts)
    int attempts = 0;
    const int max_attempts = 3;
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet = nullptr;
    int res = 0;
    while (attempts < max_attempts) {
        res = pcap_next_ex(handle, &header, &packet);
        if (res == 1) {
            // Successfully captured a packet
            std::cerr << "Captured packet of length " << header->len << std::endl;
            break;
        } else if (res == 0) {
            // timeout elapsed
            attempts++;
            continue;
        } else if (res == -1) {
            // error occurred
            std::cerr << "pcap_next_ex error: " << pcap_geterr(handle) << std::endl;
            break;
        } else if (res == -2) {
            // EOF from savefile (unlikely for live capture)
            std::cerr << "pcap_next_ex returned EOF (-2)" << std::endl;
            break;
        }
    }

    // 9) Clean up compiled filter, pcap handle and device list
    pcap_freecode(&fp);
    pcap_close(handle);
    if (alldevs) pcap_freealldevs(alldevs);

    // If we reached this point, the API sequence completed (even if no packet was captured).
    return 66;
}