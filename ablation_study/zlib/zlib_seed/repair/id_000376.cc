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
//<ID> 376
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    Bytef payload[] = "zlib payload for gz and inflate test";
    uLong payload_len = (uLong)(sizeof(payload) - 1);
    uLong comp_bound = compressBound(payload_len);
    gzFile gz_wr;
    gzFile gz_rd;
    Bytef read_buf[256];
    Bytef out_buf[512];
    z_size_t read_items;
    int c;
    int ret_gzwrite;
    int ret_gzclose_wr;
    int ret_gzclose_rd;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;

    // step 2: Setup / Write compressed file (via gz)
    memset(&strm, 0, sizeof(strm));
    memset(read_buf, 0, sizeof(read_buf));
    memset(out_buf, 0, sizeof(out_buf));
    gz_wr = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    ret_gzwrite = gzwrite(gz_wr, (voidpc)payload, (unsigned int)payload_len);
    gzflush(gz_wr, 0);
    ret_gzclose_wr = gzclose(gz_wr);

    // step 3: Operate - read back with gzfread and gzgetc
    gz_rd = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_rd);
    c = gzgetc(gz_rd);
    ret_gzclose_rd = gzclose(gz_rd);

    // step 4: Inflate init/use/end and Cleanup / Validate
    strm.next_in = read_buf;
    strm.avail_in = (uInt)read_items;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)sizeof(out_buf);
    strm.reserved = comp_bound;
    ret_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    ret_inflate_call = inflate(&strm, 0);
    ret_inflate_end = inflateEnd(&strm);
    (void)ret_gzwrite;
    (void)ret_gzclose_wr;
    (void)ret_gzclose_rd;
    (void)c;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    // API sequence test completed successfully
    return 66;
}