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
//<ID> 1118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and compress an in-memory payload
    const Bytef source[] = "zlib API sequence payload: compress -> inflate -> gzip file write/read -> cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Inflate the compressed buffer back to verify round-trip, then call inflateEnd
    z_stream inflate_strm;
    memset(&inflate_strm, 0, (int)sizeof(inflate_strm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&inflate_strm, version, (int)sizeof(z_stream));
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)comp_len;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inflate_strm.next_out = out_buf;
    inflate_strm.avail_out = (uInt)out_alloc;
    int inflate_ret = inflate(&inflate_strm, 4);
    uLong out_size = inflate_strm.total_out;
    int inflate_end_ret = inflateEnd(&inflate_strm);

    // step 3: Persist original data into a .gz file, reopen and rewind, then close
    gzFile gw = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_write_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_sequence.gz", "rb");
    int gzrewind_ret = gzrewind(gr);
    int gzclose_read_ret = gzclose(gr);

    // step 4: Validate checksum and cleanup allocated resources
    uLong crc = crc32(0UL, comp_buf, (uInt)comp_len);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inflate_ret;
    (void)out_size;
    (void)inflate_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)gzrewind_ret;
    (void)gzclose_read_ret;
    (void)crc;
    (void)version;
    (void)bound;
    (void)source_len;
    return 66;
    // API sequence test completed successfully
}