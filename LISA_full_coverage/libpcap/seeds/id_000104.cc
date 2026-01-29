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
//<ID> 104
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
    char *device = pcap_lookupdev(errbuf);
    const char *libver = pcap_lib_version();

    // step 2: Configure (avoid using pcap_alloc_option / pcap_set_option_int to prevent link errors)
    pcap_t *handle = pcap_create(device, errbuf);
    pcap_set_snaplen(handle, 65535);
    pcap_set_promisc(handle, 1);
    pcap_set_timeout(handle, 1000);
    pcap_set_immediate_mode(handle, 1);
    pcap_set_tstamp_precision(handle, 1); // replace option-setting with direct API
    pcap_activate(handle);

    // step 3: Operate / Validate
    int *dlt_list = NULL;
    int rc_list = pcap_list_datalinks(handle, &dlt_list);
    pcap_dumper_t *dumper = pcap_dump_open(handle, "test_capture.pcap");
    struct bpf_program prog;
    prog.bf_len = 0;
    prog.bf_insns = NULL;
    const char *filter_expr = "tcp";
    bpf_u_int32 netmask = 0;
    int rc_compile = pcap_compile(handle, &prog, filter_expr, 1, netmask);
    int rc_setf = pcap_setfilter(handle, &prog);
    pcap_dump_flush(dumper);

    // step 4: Cleanup
    pcap_freecode(&prog);
    pcap_dump_close(dumper);
    pcap_free_datalinks(dlt_list);
    pcap_close(handle);

    (void)errbuf;
    (void)device;
    (void)libver;
    (void)rc_list;
    (void)rc_compile;
    (void)rc_setf;
    (void)filter_expr;
    puts("API sequence test completed successfully");
    return 66;
}