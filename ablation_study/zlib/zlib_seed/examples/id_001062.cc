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
//<ID> 1062
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial compression of payload (one-shot)
    const char payload[] = "zlib API sequence payload exercising gzwrite, gzfread, deflateInit_, inflateMark and related streaming calls";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Create a gzip file via gzopen, write the original payload with gzwrite, then read it back with gzfread
    gzFile gz_w = gzopen("tmp_zlib_api_seq.gz", "wb");
    int rc_gzwrite = gzwrite(gz_w, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzclose_w = gzclose(gz_w);
    gzFile gz_r = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *read_back = new Bytef[(payload_len ? payload_len : 1)];
    z_size_t items_read = gzfread((voidp)read_back, (z_size_t)1, (z_size_t)payload_len, gz_r);
    int rc_gzclose_r = gzclose(gz_r);

    // step 3: Initialize a deflate stream (deflateInit_), run a single deflate call, and exercise inflate with inflateMark
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    Bytef *def_out = new Bytef[(bound ? bound : 1)];
    dstrm.next_out = def_out;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    Bytef *inflate_out = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = inflate_out;
    istrm.avail_out = (uInt)payload_len;
    long mark = inflateMark(&istrm);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and validate
    delete [] comp_buf;
    delete [] read_back;
    delete [] def_out;
    delete [] inflate_out;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)items_read;
    (void)rc_gzclose_r;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}