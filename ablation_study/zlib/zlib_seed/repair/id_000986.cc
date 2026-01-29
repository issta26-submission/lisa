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
//<ID> 986
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

    // step 2: Configure
    int init_ret = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    int prime_ret = inflatePrime(&strm, 3, 5);
    uInt dict_len = (uInt)64;
    Bytef *dict_buf = (Bytef *)malloc((size_t)dict_len);
    memset(dict_buf, 0, (size_t)dict_len);

    // step 3: Operate
    int getdict_ret = inflateGetDictionary(&strm, dict_buf, &dict_len);

    // step 4: Validate / Cleanup
    int backend_ret = inflateBackEnd(&strm);
    free(window);
    free(dict_buf);
    (void)version;
    (void)init_ret;
    (void)prime_ret;
    (void)getdict_ret;
    (void)backend_ret;
    // API sequence test completed successfully
    return 66;
}