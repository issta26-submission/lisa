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
//<ID> 1072
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, compress payload into a buffer
    const char payload[] = "zlib API sequence payload exercising compress2, gzopen/gzread/gzclose, inflateMark, gzclearerr";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Write the compressed bytes into a .gz file (gz layer will wrap/compress the provided bytes)
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = 0;
    if (gzw) rc_gzwrite = gzwrite(gzw, (const void *)comp_buf, (unsigned int)comp_len_f);
    int rc_gzclose_w = 0;
    if (gzw) rc_gzclose_w = gzclose(gzw);

    // step 3: Re-open the file for reading, read decompressed content (which yields the original compressed bytes),
    //         clear any gz errors, then close the reader
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = new Bytef[(comp_len_f ? comp_len_f : 1)];
    int rc_gzread = 0;
    if (gzr) rc_gzread = gzread(gzr, (voidp)read_buf, (unsigned int)comp_len_f);
    gzclearerr(gzr);
    int rc_gzclose_r = 0;
    if (gzr) rc_gzclose_r = gzclose(gzr);

    // step 4: Initialize an inflate stream, query its mark for validation, then cleanup streams and buffers
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    long mark = inflateMark(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    delete [] comp_buf;
    delete [] read_buf;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_init;
    (void)mark;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}