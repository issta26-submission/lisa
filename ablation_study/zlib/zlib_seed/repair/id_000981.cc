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
//<ID> 981
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    Bytef *comp_buf = (Bytef *)malloc(64);
    memset(comp_buf, 0, 64);
    back_strm.next_in = comp_buf;
    back_strm.avail_in = (uInt)64;

    // step 2: Configure
    int init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int prime_ret = inflatePrime(&back_strm, 3, 5);

    // step 3: Operate
    Bytef *dict = (Bytef *)malloc(64);
    memset(dict, 0, 64);
    uInt dictLen = (uInt)64;
    int dict_ret = inflateGetDictionary(&back_strm, dict, &dictLen);

    // step 4: Validate / Cleanup
    int end_ret = inflateBackEnd(&back_strm);
    free(window);
    free(comp_buf);
    free(dict);
    (void)version;
    (void)init_ret;
    (void)prime_ret;
    (void)dict_ret;
    (void)dictLen;
    (void)end_ret;
    // API sequence test completed successfully
    return 66;
}