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
//<ID> 960
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib payload for gzwrite/gzprintf and adler32_combine test";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLongf comp_len_f = 0;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Configure gzFile for output
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 8192);

    // step 3: Operate - write original payload and compressed blob, add a simple annotation
    int rc_gzwrite1 = gzwrite(gf, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzwrite2 = gzwrite(gf, (voidpc)comp_buf, (unsigned int)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u));
    int rc_gzprintf = gzprintf(gf, "zlib-test-marker\n");
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and cleanup
    uLong ad1 = adler32(0UL, src_buf, (uInt)payload_len);
    uLong ad2 = adler32(1UL, src_buf, (uInt)payload_len);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)payload_len);
    delete [] comp_buf;

    (void)rc_compress;
    (void)comp_size;
    (void)rc_gzbuffer;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_gzprintf;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    // API sequence test completed successfully
    return 66;
}