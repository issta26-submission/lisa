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
//<ID> 963
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial compression of payload
    const char payload[] = "zlib API sequence payload exercising adler32_combine, gzbuffer, gzprintf and gzwrite";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];

    // step 2: Initialize inflate stream and perform inflation
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Compute Adler-32 checksums and combine them
    uLong ad1 = adler32(0UL, src_buf, (uInt)payload_len);
    uLong ad2 = adler32(1UL, src_buf, (uInt)payload_len);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)payload_len);

    // step 4: Write original payload to gzFile with buffering and formatted output, then cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 4096);
    int rc_gzprintf = gzprintf(gf, "PAYLOAD:");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzclose = gzclose(gf);

    int rc_inflate_end = inflateEnd(&istrm);

    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_compress;
    (void)comp_size;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)rc_gzbuffer;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}