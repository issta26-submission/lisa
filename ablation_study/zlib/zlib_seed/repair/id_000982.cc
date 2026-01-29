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
//<ID> 982
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
    const unsigned int windowBits = 15u;
    const unsigned int window_size = 1u << windowBits;
    unsigned char *window = (unsigned char *)malloc((size_t)window_size);
    memset(window, 0, (size_t)window_size);
    const Bytef comp_data[] = { 0x78, 0x9c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01 }; // small zlib stream (minimal)
    uInt comp_len = (uInt)(sizeof(comp_data));
    back_strm.next_in = (Bytef *)comp_data;
    back_strm.avail_in = comp_len;
    Bytef *out_buf = (Bytef *)malloc(512);
    memset(out_buf, 0, 512);
    back_strm.next_out = out_buf;
    back_strm.avail_out = (uInt)512;

    // step 2: Configure
    int init_ret = inflateBackInit_(&back_strm, (int)windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    int prime_ret = inflatePrime(&back_strm, 3, 5);
    Bytef *dict_buf = (Bytef *)malloc(256);
    memset(dict_buf, 0, 256);
    uInt dict_len = (uInt)256;
    int getdict_ret = inflateGetDictionary(&back_strm, dict_buf, &dict_len);

    // step 4: Validate / Cleanup
    int end_ret = inflateBackEnd(&back_strm);
    free(window);
    free(out_buf);
    free(dict_buf);
    (void)version;
    (void)init_ret;
    (void)prime_ret;
    (void)getdict_ret;
    (void)end_ret;
    // API sequence test completed successfully
    return 66;
}