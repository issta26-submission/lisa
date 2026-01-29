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
//<ID> 1535
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and deflate stream)
    const Bytef src[] = "zlib API sequence test: gzopen64, gzfwrite, deflateTune, crc32 and gzseek";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char * ver = zlibVersion();

    // step 2: Configure (open gzip file and write the source data via gzfwrite)
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    z_size_t written_items = gzfwrite((voidpc)src, (z_size_t)1, (z_size_t)src_len, gz);

    // step 3: Operate (compute CRC of the source, seek in the gz file, initialize and tune deflate)
    uLong computed_crc = crc32(0UL, src, (uInt)src_len);
    off_t seek_pos = gzseek(gz, (off_t)0, 0);
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    int good_length = (int)(src_len % 256u);
    int max_lazy = (int)((src_len / 2u) % 101u);
    int nice_length = good_length + 3;
    int max_chain = (int)((src_len % 50u) + 1u);
    int tune_ret = deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);

    // step 4: Validate & Cleanup (end deflate, close file, and silence unused results)
    int def_end_ret = deflateEnd(&strm);
    int gz_close_ret = gzclose(gz);
    (void)ver;
    (void)written_items;
    (void)computed_crc;
    (void)seek_pos;
    (void)def_init_ret;
    (void)tune_ret;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}