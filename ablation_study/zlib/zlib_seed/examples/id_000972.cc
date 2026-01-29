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
//<ID> 972
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and checksum computation
    const char payload[] = "zlib API sequence payload exercising adler32_z, gzprintf, gzoffset64 and inflateInit_";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong ad_z = adler32_z(0UL, src_buf, (z_size_t)payload_len);

    // step 2: Compress payload to produce input for inflate
    uLong bound = compressBound((uLong)payload_len);
    uLongf comp_len_f = (uLongf)bound;
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 3: Initialize inflate stream and perform inflation (Initialize -> Operate -> Validate)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Write original payload to gzFile, query offset, and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzprintf = gzprintf(gf, payload);
    off64_t off = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)ad_z;
    (void)rc_compress;
    (void)comp_size;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_gzprintf;
    (void)off;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}