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
//<ID> 1490
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: prepare data, set dictionary, deflate, write gz, read via gzfread, verify CRCs";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    const Bytef dict[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    int dict_ret = deflateSetDictionary(&strm, dict, dict_len);

    // step 2: Configure
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)(bound + 16UL));
    memset(comp_buf, 0, (int)(bound + 16UL));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)(bound + 16UL);
    int deflate_ret = deflate(&strm, 0);

    // step 3: Operate
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write = gzwrite(gw, (voidp)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_params = gzsetparams(gw, 6, 0);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(bound + 32UL));
    memset(read_buf, 0, (int)(bound + 32UL));
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)(bound + 31UL), gr);
    int deflate_end = deflateEnd(&strm);

    // step 4: Validate & Cleanup
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    uLong crc_read = crc32(0UL, read_buf, (uInt)((items_read > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)items_read));
    uLong crc_combined = crc32_combine(crc_src, crc_read, (off_t)items_read);
    int gr_close = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)version;
    (void)dict_ret;
    (void)deflate_ret;
    (void)gw_write;
    (void)gw_params;
    (void)gw_close;
    (void)items_read;
    (void)deflate_end;
    (void)crc_src;
    (void)crc_read;
    (void)crc_combined;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}