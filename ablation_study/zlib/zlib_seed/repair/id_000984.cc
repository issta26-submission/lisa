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
//<ID> 984
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
    unsigned char window[32768];
    memset(window, 0, sizeof(window));

    // step 2: Configure
    int init_ret = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    int prime_ret1 = inflatePrime(&strm, 0, 0);
    Bytef dict_buf[128];
    memset(dict_buf, 0, sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int getdict_ret = inflateGetDictionary(&strm, dict_buf, &dict_len);

    // step 3: Operate
    int prime_ret2 = inflatePrime(&strm, 3, 5);
    window[0] = dict_buf[0];
    window[1] = (unsigned char)(dict_len & 0xFF);

    // step 4: Cleanup
    int end_ret = inflateBackEnd(&strm);
    (void)init_ret;
    (void)prime_ret1;
    (void)getdict_ret;
    (void)prime_ret2;
    (void)end_ret;
    (void)version;
    (void)dict_len;

    // API sequence test completed successfully
    return 66;
}