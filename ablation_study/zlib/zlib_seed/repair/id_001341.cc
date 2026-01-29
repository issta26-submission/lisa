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
//<ID> 1341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (source and version)
    const Bytef src[] = "Hello zlib sequence";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Configure (initialize deflate stream and allocate output buffer)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm, src_len);
    Bytef *outbuf = (Bytef *)malloc((size_t)bound);
    memset(outbuf, 0, (int)bound);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate (compress, finalize, write compressed data to a .gz file)
    int def_ret = deflate(&strm, 4);
    uLong out_len = (uLong)strm.total_out;
    int end_ret = deflateEnd(&strm);
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, outbuf, (unsigned int)out_len);
    int close_w = gzclose(gz_w);

    // step 4: Validate & Cleanup (reopen file, seek, read back, check EOF, close and free)
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_ret = gzseek(gz_r, (off_t)0, 0);
    Bytef *readbuf = (Bytef *)malloc((size_t)src_len);
    memset(readbuf, 0, (int)src_len);
    int gr = gzread(gz_r, readbuf, (unsigned int)src_len);
    int eof_flag = gzeof(gz_r);
    int close_r = gzclose(gz_r);
    free(outbuf);
    free(readbuf);

    (void)init_ret;
    (void)def_ret;
    (void)end_ret;
    (void)gw;
    (void)close_w;
    (void)seek_ret;
    (void)gr;
    (void)eof_flag;
    (void)close_r;
    (void)version;
    // API sequence test completed successfully
    return 66;
}