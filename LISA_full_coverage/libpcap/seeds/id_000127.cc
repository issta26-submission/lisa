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
//<ID> 127
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
    pcap_if_t *alldevs = NULL;
    pcap_findalldevs(&alldevs, errbuf);
    char *device = pcap_lookupdev(errbuf);
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    pcap_lookupnet(device, &net, &mask, errbuf);
    const char *libver = pcap_lib_version();

    // step 2: Configure
    pcap_t *handle = pcap_create(device, errbuf);
    pcap_set_snaplen(handle, 65535);
    pcap_set_promisc(handle, 1);
    pcap_set_timeout(handle, 1000);
    int dlt = pcap_datalink_name_to_val("EN10MB");
    pcap_set_datalink(handle, dlt);
    int *tstamp_types = NULL;
    pcap_list_tstamp_types(handle, &tstamp_types);
    int chosen_tstamp = tstamp_types[0];
    pcap_set_tstamp_type(handle, chosen_tstamp);
    const char *tstamp_name = pcap_tstamp_type_val_to_name(chosen_tstamp);
    const char *dlt_name = pcap_datalink_val_to_name(dlt);
    pcap_activate(handle);

    // step 3: Operate / Validate
    struct bpf_program fp;
    fp.bf_len = 0;
    fp.bf_insns = NULL;
    pcap_compile(handle, &fp, "tcp", 1, net);
    pcap_setfilter(handle, &fp);
    pcap_dumper_t *dumper = pcap_dump_open(handle, "capture_output.pcap");
    int current_dlt = pcap_datalink(handle);
    const char *current_dlt_name = pcap_datalink_val_to_name(current_dlt);
    pcap_dump_close(dumper);

    // step 4: Cleanup
    pcap_freecode(&fp);
    pcap_free_tstamp_types(tstamp_types);
    pcap_freealldevs(alldevs);
    pcap_close(handle);
    (void)libver;
    (void)device;
    (void)errbuf;
    (void)net;
    (void)mask;
    (void)dlt;
    (void)tstamp_name;
    (void)dlt_name;
    (void)current_dlt_name;
    puts("API sequence test completed successfully");
    return 66;
}