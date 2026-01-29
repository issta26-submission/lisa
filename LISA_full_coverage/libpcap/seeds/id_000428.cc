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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // step 1: Initialize
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));
    const char device[] = "any";
    pcap_t *handle = pcap_create(device, errbuf);

    // step 2: Configure
    int rc_snaplen = pcap_set_snaplen(handle, 65535);
    int rc_promisc = pcap_set_promisc(handle, 1);
    int rc_timeout = pcap_set_timeout(handle, 1000);
    int rc_tstampprec_set = pcap_set_tstamp_precision(handle, 1);
    int rc_activate = pcap_activate(handle);

    // step 3: Operate & Validate
    int fd = pcap_fileno(handle);
    struct pcap_pkthdr *hdr = NULL;
    const u_char *packet = NULL;
    int rc_next = pcap_next_ex(handle, &hdr, &packet);
    FILE *fp = fopen("libpcap_test_output.pcap", "wb");
    pcap_dumper_t *dumper = pcap_dump_fopen(handle, fp);
    pcap_dump((u_char *)dumper, hdr, packet);
    int rc_flush = pcap_dump_flush(dumper);
    const char *activate_msg = pcap_strerror(rc_activate);

    // step 4: Cleanup
    pcap_dump_close(dumper);
    // Do not call fclose(fp) here to avoid a double-close/double-free:
    // pcap_dump_close may close internal FILE*; closing it again can cause a double free.
    pcap_close(handle);
    (void)device;
    (void)errbuf;
    (void)rc_snaplen;
    (void)rc_promisc;
    (void)rc_timeout;
    (void)rc_tstampprec_set;
    (void)fd;
    (void)hdr;
    (void)packet;
    (void)rc_next;
    (void)rc_flush;
    (void)activate_msg;
    puts("API sequence test completed successfully");
    return 66;
}