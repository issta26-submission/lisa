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
//<ID> 1095
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for deflate, gz write, and inflateBackInit";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize deflate stream, compute bound, and perform deflate
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong max_comp = deflateBound(&dstrm, payload_len);
    size_t comp_buf_sz = (size_t)(max_comp ? max_comp : 1);
    Bytef *comp_buf = new Bytef[comp_buf_sz];
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_buf_sz;
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    uLong comp_len = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Write compressed data to a gz file, query position, and initialize inflateBack
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = 0;
    if (gw) rc_gzwrite = gzwrite(gw, (voidpc)comp_buf, (unsigned int)(comp_len ? comp_len : 0));
    off_t pos_after_write = gztell(gw);
    int rc_gzclose = gzclose(gw);
    const int window_size = 32768;
    unsigned char *window_buf = new unsigned char[window_size];
    int rc_inflateBack_init = inflateBackInit_(&istrm, 15, window_buf, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflateBack_end = inflateBackEnd(&istrm);

    // step 4: Validate via checksum, cleanup, and silence unused warnings
    uLong adler_payload = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong adler_comp = adler32(0UL, comp_buf, (uInt)(comp_len ? comp_len : 1));
    delete [] comp_buf;
    delete [] window_buf;
    (void)rc_deflate_init;
    (void)max_comp;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)rc_gzclose;
    (void)rc_inflateBack_init;
    (void)rc_inflateBack_end;
    (void)adler_payload;
    (void)adler_comp;
    // API sequence test completed successfully
    return 66;
}