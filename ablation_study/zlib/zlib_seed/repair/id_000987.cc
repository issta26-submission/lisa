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
//<ID> 987
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
    int prime_ret = inflatePrime(&strm, 0, 0);
    uInt dictCap = (uInt)128;
    Bytef *dictBuf = (Bytef *)malloc((size_t)dictCap);
    memset(dictBuf, 0, (size_t)dictCap);

    // step 3: Operate
    int getdict_ret = inflateGetDictionary(&strm, dictBuf, &dictCap);

    // step 4: Validate / Cleanup
    int backend_ret = inflateBackEnd(&strm);
    free(window);
    free(dictBuf);
    (void)version;
    (void)init_ret;
    (void)prime_ret;
    (void)getdict_ret;
    (void)backend_ret;
    // API sequence test completed successfully
    return 66;
}