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
//<ID> 1067
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and compress payload into a buffer
    const char payload[] = "zlib API sequence payload exercising deflateInit_, inflateMark, gzwrite, gzfread";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? (size_t)bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Write compressed data to a gz file and read it back with gzfread
    gzFile wf = gzopen("test_zlib_api_seq.bin", "wb");
    int rc_gzwrite = gzwrite(wf, comp_buf, (unsigned int)comp_len_f);
    int rc_gzclose_w = gzclose(wf);
    gzFile rf = gzopen("test_zlib_api_seq.bin", "rb");
    Bytef *read_back = new Bytef[(comp_len_f ? (size_t)comp_len_f : 1)];
    z_size_t items_read = gzfread(read_back, (z_size_t)1, (z_size_t)comp_len_f, rf);
    int rc_gzclose_r = gzclose(rf);

    // step 3: Inflate the read-back compressed data and record inflateMark; then deflate the inflated output
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_back;
    istrm.avail_in = (uInt)items_read;
    Bytef *inflated_out = new Bytef[(payload_len ? (size_t)payload_len : 1)];
    istrm.next_out = inflated_out;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate = inflate(&istrm, 0);
    long mark = inflateMark(&istrm);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = inflated_out;
    dstrm.avail_in = (uInt)payload_len;
    uLong def_bound = compressBound((uLong)payload_len);
    Bytef *def_out = new Bytef[(def_bound ? (size_t)def_bound : 1)];
    dstrm.next_out = def_out;
    dstrm.avail_out = (uInt)(def_bound ? (uInt)def_bound : 1U);
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Cleanup and validate
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] read_back;
    delete [] inflated_out;
    delete [] def_out;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzclose_r;
    (void)items_read;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)mark;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}