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
//<ID> 1063
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Do not change the function name or signature.
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, compress payload into comp_buf
    const char payload[] = "zlib API sequence payload exercising gzwrite, gzfread, deflateInit_, inflateMark";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Write compressed data to a gz file and read it back via gzfread
    gzFile gf = gzopen("test_zlib_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, comp_buf, (unsigned int)comp_len_f);
    int rc_gzclose_w = gzclose(gf);
    gzFile gf_r = gzopen("test_zlib_tmp.gz", "rb");
    Bytef *read_buf = new Bytef[(comp_len_f ? (uLong)comp_len_f : 1)];
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)comp_len_f, gf_r);
    int rc_gzclose_r = gzclose(gf_r);

    // step 3: Initialize inflate stream and call inflateMark, then initialize deflate stream and run deflate
    z_stream istrm;
    std::memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)items_read;
    long mark = inflateMark(&istrm);

    z_stream dstrm;
    std::memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf; // cast away const to match API (keeps original logic)
    dstrm.avail_in = (uInt)payload_len;
    uLong out_bound = compressBound((uLong)payload_len);
    Bytef *out_buf = new Bytef[(out_bound ? out_bound : 1)];
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)(out_bound ? out_bound : 1);
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Validate (record totals) and cleanup
    uLong total_in_inflate = istrm.total_in;
    uLong total_out_deflate = dstrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] read_buf;
    delete [] out_buf;

    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzclose_r;
    (void)items_read;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)total_in_inflate;
    (void)total_out_deflate;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}