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
//<ID> 980
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

    // step 2: Configure (initialize inflateBack with a sliding window)
    int init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (prime bit buffer and attempt to obtain any preset dictionary)
    int prime_ret = inflatePrime(&back_strm, 3, 5);
    uInt dict_capacity = (uInt)32768;
    Bytef *dict_buf = (Bytef *)malloc((size_t)dict_capacity);
    memset(dict_buf, 0, (size_t)dict_capacity);
    uInt dict_len = dict_capacity;
    int getdict_ret = inflateGetDictionary(&back_strm, dict_buf, &dict_len);
    uLong dict_adler = adler32(0UL, dict_buf, dict_len);

    // step 4: Validate / Cleanup
    int back_end_ret = inflateBackEnd(&back_strm);
    free(window);
    free(dict_buf);
    (void)version;
    (void)init_ret;
    (void)prime_ret;
    (void)getdict_ret;
    (void)dict_len;
    (void)dict_capacity;
    (void)dict_adler;
    (void)back_end_ret;
    // API sequence test completed successfully
    return 66;
}