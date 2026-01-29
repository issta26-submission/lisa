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
//<ID> 1065
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compress it with compress2
    const char payload[] = "zlib API sequence payload exercising gzwrite, gzfread, deflateInit_, inflateMark";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Write compressed data to a gz file and read it back using gzwrite and gzfread
    const char fname[] = "test_zlib_api_seq.gz";
    gzFile gf_write = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gf_write, (voidpc)comp_buf, (unsigned int)comp_len_f);
    int rc_gzclose_write = gzclose(gf_write);
    gzFile gf_read = gzopen(fname, "rb");
    Bytef *comp_buf_read = new Bytef[(bound ? bound : 1)];
    z_size_t read_items = gzfread((voidp)comp_buf_read, (z_size_t)1, (z_size_t)bound, gf_read);
    int rc_gzclose_read = gzclose(gf_read);

    // step 3: Initialize deflate and inflate streams, perform operations and inspect inflate mark
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = comp_buf_read;
    dstrm.avail_in = (uInt)(read_items ? read_items : 0);
    Bytef *def_out = new Bytef[(payload_len ? payload_len : 1)];
    dstrm.next_out = def_out;
    dstrm.avail_out = (uInt)payload_len;
    int rc_deflate = deflate(&dstrm, 0);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf_read;
    istrm.avail_in = (uInt)(read_items ? read_items : 0);
    Bytef *infl_out = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = infl_out;
    istrm.avail_out = (uInt)payload_len;
    long mark_before = inflateMark(&istrm);

    // step 4: Cleanup and finalize
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] comp_buf_read;
    delete [] def_out;
    delete [] infl_out;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_write;
    (void)rc_gzclose_read;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_inflate_init;
    (void)mark_before;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}