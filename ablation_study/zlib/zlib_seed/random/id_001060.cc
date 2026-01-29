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
//<ID> 1060
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload
    const char payload[] = "zlib API sequence payload exercising deflate, gzwrite, gzfread, inflateInit_, inflateMark, inflate";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));

    // step 2: Compress payload with deflate (stream API)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);
    uLong comp_len = dstrm.total_out;

    // step 3: Write compressed data to a gz file and read it back
    gzFile gf_out = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gf_out, comp_buf, (unsigned int)comp_len);
    int rc_gzclose_out = gzclose(gf_out);
    gzFile gf_in = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = new Bytef[(comp_len ? comp_len : 1)];
    memset(read_buf, 0, (size_t)(comp_len ? comp_len : 1));
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)comp_len, gf_in);
    int rc_gzclose_in = gzclose(gf_in);

    // step 4: Initialize inflate stream, mark and inflate, then cleanup
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)items_read;
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    memset(out_buf, 0, (size_t)(payload_len ? payload_len : 1));
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)payload_len;
    long mark = inflateMark(&istrm);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    delete [] comp_buf;
    delete [] read_buf;
    delete [] out_buf;

    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_out;
    (void)rc_gzclose_in;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}