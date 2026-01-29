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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    const char *dev = NULL;

    /* Find all devices */
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        /* fallback to lookupdev if findalldevs fails */
        dev = pcap_lookupdev(errbuf);
        if (!dev) {
            fprintf(stderr, "pcap_findalldevs and pcap_lookupdev failed: %s\n", errbuf);
            return -1;
        }
    } else {
        /* If findalldevs succeeded, pick the first device if available */
        if (alldevs == NULL) {
            fprintf(stderr, "pcap_findalldevs returned no devices\n");
            return -2;
        }
        dev = alldevs->name;
    }

    /* Open the chosen device for live capture */
    pcap_t *handle = pcap_open_live(dev, BUFSIZ, 1 /*promisc*/, 1000 /*ms timeout*/, errbuf);
    if (!handle) {
        fprintf(stderr, "pcap_open_live failed on device %s: %s\n", dev ? dev : "(null)", errbuf);
        if (alldevs) pcap_freealldevs(alldevs);
        return -3;
    }

    /* Query the link-layer header type */
    int linktype = pcap_datalink(handle);
    (void)linktype; /* used to demonstrate usage; not required further */

    /* Compile and set a simple BPF filter */
    struct bpf_program fp;
    const char *filter_exp = "tcp and port 80";
    if (pcap_compile(handle, &fp, filter_exp, 1 /*optimize*/, PCAP_NETMASK_UNKNOWN) == -1) {
        fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -4;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -5;
    }
    pcap_freecode(&fp);

    /* Demonstrate writing a packet to a pcap file using a dead capture handle */
    pcap_t *dead = pcap_open_dead(DLT_EN10MB, 65535);
    if (!dead) {
        fprintf(stderr, "pcap_open_dead failed\n");
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -6;
    }

    pcap_dumper_t *dumper = pcap_dump_open(dead, "test_output.pcap");
    if (!dumper) {
        fprintf(stderr, "pcap_dump_open failed: %s\n", pcap_geterr(dead));
        pcap_close(dead);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -7;
    }

    /* Create a dummy packet and write it */
    const u_char dummy_pkt[60] = {0};
    struct pcap_pkthdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.ts.tv_sec = (bpf_u_int32)time(NULL);
    hdr.ts.tv_usec = 0;
    hdr.caplen = hdr.len = (bpf_u_int32)sizeof(dummy_pkt);

    pcap_dump((u_char *)dumper, &hdr, dummy_pkt);

    /* Cleanup */
    pcap_dump_close(dumper);
    pcap_close(dead);
    pcap_close(handle);
    if (alldevs) pcap_freealldevs(alldevs);

    /* Indicate success as requested */
    return 66;
}