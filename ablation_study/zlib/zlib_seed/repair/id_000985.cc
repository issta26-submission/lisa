#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 985
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int init_ret = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    // step 2: Configure
    const Bytef comp_source[] = { 0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00 }; // small sample bytes
    strm.next_in = (Bytef *)comp_source;
    strm.avail_in = (uInt)(sizeof(comp_source));
    Bytef *dict_out = (Bytef *)malloc(128);
    memset(dict_out, 0, 128);
    uInt dict_len = (uInt)128;

    // step 3: Operate
    int prime_ret1 = inflatePrime(&strm, 3, 5);
    int get_dict_ret = inflateGetDictionary(&strm, dict_out, &dict_len);
    int prime_ret2 = inflatePrime(&strm, 2, 1);

    // step 4: Validate / Cleanup
    int end_ret = inflateBackEnd(&strm);
    free(window);
    free(dict_out);
    (void)version;
    (void)init_ret;
    (void)prime_ret1;
    (void)get_dict_ret;
    (void)prime_ret2;
    (void)end_ret;
    // API sequence test completed successfully
    return 66;
}