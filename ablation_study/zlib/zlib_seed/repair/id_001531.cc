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
//<ID> 1531
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data, checksum and compression stream)
    const Bytef src[] = "zlib API sequence test: deflateTune, crc32, gzopen64, gzfwrite and gzseek usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong src_crc = crc32(0L, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure (allocate output buffer using deflateBound and tune deflate)
    uLong out_bound = deflateBound(&strm, src_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (int)out_bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_bound;
    int tune_ret = deflateTune(&strm, 16, 64, 128, 256);

    // step 3: Operate (perform a deflate call, write compressed bytes to gzip file and seek)
    int def_ret = deflate(&strm, 0);
    uLong produced = strm.total_out;
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    z_size_t written = gzfwrite((voidpc)out_buf, (z_size_t)1, (z_size_t)produced, gz);
    off_t seek_ret = gzseek(gz, (off_t)0, 0);

    // step 4: Validate & Cleanup (end deflate, close file, free buffers and silence results)
    int def_end_ret = deflateEnd(&strm);
    int gz_close_ret = gzclose(gz);
    free(out_buf);
    (void)src_crc;
    (void)def_init_ret;
    (void)ver;
    (void)out_bound;
    (void)tune_ret;
    (void)def_ret;
    (void)produced;
    (void)written;
    (void)seek_ret;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}