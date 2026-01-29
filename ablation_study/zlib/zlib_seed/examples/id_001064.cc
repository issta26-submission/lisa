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
//<ID> 1064
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and compress payload
    const char payload[] = "zlib API sequence payload exercising gzwrite, gzfread, deflateInit_, inflateMark";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Write compressed data to a gzip file and read it back using gzfread
    gzFile gw = gzopen("zlib_seq_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, comp_buf, (unsigned int)comp_len_f);
    int rc_gw_close = gzclose(gw);
    gzFile gr = gzopen("zlib_seq_tmp.gz", "rb");
    Bytef *read_buf = new Bytef[(comp_len_f ? (uLong)comp_len_f : 1)];
    z_size_t read_count = gzfread(read_buf, (z_size_t)1, (z_size_t)comp_len_f, gr);
    int rc_gr_close = gzclose(gr);

    // step 3: Inflate the read compressed data and retrieve the inflate mark
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)read_count;
    Bytef *decomp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate = inflate(&istrm, 0);
    long mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Deflate the decompressed data, cleanup, and finalize
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = decomp_buf;
    dstrm.avail_in = (uInt)payload_len;
    uLong re_bound = deflateBound(&dstrm, (uLong)payload_len);
    Bytef *re_comp = new Bytef[(re_bound ? re_bound : 1)];
    dstrm.next_out = re_comp;
    dstrm.avail_out = (uInt)re_bound;
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    delete [] comp_buf;
    delete [] read_buf;
    delete [] decomp_buf;
    delete [] re_comp;

    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gw_close;
    (void)read_count;
    (void)rc_gr_close;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)mark;
    (void)rc_inflate_end;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}