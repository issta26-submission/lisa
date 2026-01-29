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
//<ID> 349
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    z_stream inf_strm;
    Bytef src_buf[] = "Zlib API sequence payload for inflate and gz file operations.";
    uLong src_len = (uLong)(sizeof(src_buf) - 1);
    Bytef comp_buf[4096];
    Bytef decomp_buf[4096];
    uLongf comp_len_f;
    uLong decomp_len;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    gzFile gz_write;
    gzFile gz_read;
    int ret_gzwrite;
    int ret_gzclose_write;
    int ret_gzbuffer;
    int ret_gzeof;
    int ret_gzclose_r;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(decomp_buf, 0, sizeof(decomp_buf));
    comp_len_f = (uLongf)sizeof(comp_buf);
    ret_compress = compress(comp_buf, &comp_len_f, (const Bytef *)src_buf, src_len);

    // step 3: Core operations - initialize inflate and perform inflate
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len_f;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: GZ file operations and cleanup
    gz_write = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gz_write, (voidpc)comp_buf, (unsigned int)comp_len_f);
    ret_gzclose_write = gzclose(gz_write);
    gz_read = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    ret_gzbuffer = gzbuffer(gz_read, 8192);
    ret_gzeof = gzeof(gz_read);
    ret_gzclose_r = gzclose_r(gz_read);

    (void)ver;
    (void)src_len;
    (void)comp_len_f;
    (void)decomp_len;
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_gzwrite;
    (void)ret_gzclose_write;
    (void)ret_gzbuffer;
    (void)ret_gzeof;
    (void)ret_gzclose_r;
    // API sequence test completed successfully
    return 66;
}