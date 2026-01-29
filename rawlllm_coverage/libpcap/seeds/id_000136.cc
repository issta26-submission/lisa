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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // Include headers here so the function is self-contained when pasted into a file.
    // (Preprocessor directives are allowed anywhere in the source.)
    #include <pcap/pcap.h>
    #include <cstdio>
    #include <cstring>

    pcap_if_t *alldevs = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        return -1;
    }

    if (alldevs == nullptr) {
        std::fprintf(stderr, "No devices found by pcap_findalldevs\n");
        return -2;
    }

    // Pick the first device found
    pcap_if_t *dev = alldevs;
    if (dev->name == nullptr) {
        std::fprintf(stderr, "First device has no name\n");
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 2) Create a pcap handle for the device and set parameters
    pcap_t *handle = pcap_create(dev->name, errbuf);
    if (handle == nullptr) {
        std::fprintf(stderr, "pcap_create failed for device %s: %s\n", dev->name, errbuf);
        pcap_freealldevs(alldevs);
        return -4;
    }

    if (pcap_set_snaplen(handle, 65535) != 0) {
        std::fprintf(stderr, "pcap_set_snaplen failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }

    if (pcap_set_promisc(handle, 1) != 0) {
        std::fprintf(stderr, "pcap_set_promisc failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    if (pcap_set_timeout(handle, 1000) != 0) { // milliseconds
        std::fprintf(stderr, "pcap_set_timeout failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -7;
    }

    // 3) Activate the handle
    int activate_ret = pcap_activate(handle);
    if (activate_ret < 0) {
        // negative return indicates an error
        std::fprintf(stderr, "pcap_activate failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -8;
    } else if (activate_ret > 0) {
        // positive return is a warning; continue but report it
        std::fprintf(stderr, "pcap_activate returned warning code %d: %s\n", activate_ret, pcap_geterr(handle));
    }

    // 4) Optionally lookup network/mask for compiling a filter
    bpf_u_int32 net = 0, mask = 0;
    if (pcap_lookupnet(dev->name, &net, &mask, errbuf) == -1) {
        // not fatal for our simple test; set net/mask to 0
        std::fprintf(stderr, "pcap_lookupnet failed for %s: %s\n", dev->name, errbuf);
        net = mask = 0;
    }

    // 5) Compile and set a simple BPF filter (capture TCP packets)
    struct bpf_program fp;
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1 /* optimize */, mask) == -1) {
        std::fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -9;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        std::fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -10;
    }

    // 6) Attempt to read a single packet (with a few attempts to allow timeouts)
    const u_char *packet_data = nullptr;
    struct pcap_pkthdr *pkt_header = nullptr;
    int attempts = 0;
    const int max_attempts = 5;
    int got_packet = 0;

    while (attempts < max_attempts) {
        int res = pcap_next_ex(handle, &pkt_header, &packet_data);
        if (res == 1) {
            // packet read
            got_packet = 1;
            // For testing purposes, we won't parse the packet further.
            break;
        } else if (res == 0) {
            // timeout expired; try again
            attempts++;
            continue;
        } else if (res == -1) {
            std::fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
            break;
        } else if (res == -2) {
            std::fprintf(stderr, "pcap_next_ex: no more packets (EOF)\n");
            break;
        } else {
            std::fprintf(stderr, "pcap_next_ex returned unexpected result %d\n", res);
            break;
        }
    }

    // 7) Cleanup
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    if (!got_packet) {
        // We still consider the API sequence successful even if no packet was captured,
        // because we exercised findalldevs, create/activate, compile/setfilter and read API.
        // Return success code as requested.
    }

    // Return the required success value
    return 66;
}