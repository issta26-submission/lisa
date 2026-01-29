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
//<ID> 1803
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> get dictionary -> inflateBack init -> gz write -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream dstrm;
    z_stream ibstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&ibstrm, 0, (int)sizeof(ibstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Configure and compress
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;
    uInt dict_buf_size = (uInt)256;
    Bytef *dict_buf = (Bytef *)malloc((size_t)dict_buf_size);
    memset(dict_buf, 0, (int)dict_buf_size);
    int def_getdict_ret = deflateGetDictionary(&dstrm, dict_buf, &dict_buf_size);
    int def_end_ret = deflateEnd(&dstrm);

    // step 3: InflateBackInit_ and gz write/close
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, 32768);
    int ib_init_ret = inflateBackInit_(&ibstrm, 15, window, ver, (int)sizeof(z_stream));
    int ib_end_ret = inflateBackEnd(&ibstrm);
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t wrote = gzfwrite(comp_buf, 1, (z_size_t)comp_len, gz);
    int gzflush_ret = gzflush(gz, 0);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    uLong check_adler = adler32(0UL, comp_buf, (uInt)(comp_len > 0 ? (uInt)comp_len : 0U));
    free(comp_buf);
    free(dict_buf);
    free(window);
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_getdict_ret;
    (void)def_end_ret;
    (void)ib_init_ret;
    (void)ib_end_ret;
    (void)wrote;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)check_adler;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}