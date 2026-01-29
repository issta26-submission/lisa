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
//<ID> 966
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload + compress
    const char payload[] = "zlib API sequence payload to exercise gzbuffer, gzprintf, gzwrite and adler32_combine";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Open gzFile and configure buffering and write header and payload
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 4096);
    int rc_gzprintf = gzprintf(gf, "ZLIB_PAYLOAD_HEADER\n");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));

    // step 3: Compute adler checksums and combine them to simulate validation of combined data
    uLong ad1 = adler32(0UL, src_buf, (uInt)payload_len);
    uLong ad2 = adler32(1UL, comp_buf, (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u));
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)payload_len);

    // step 4: Flush and cleanup resources
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);
    delete [] comp_buf;

    (void)rc_compress;
    (void)rc_gzbuffer;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)rc_gzflush;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}