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
//<ID> 964
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize - prepare payload and compute partial checksums
    const char payload[] = "zlib API sequence payload to exercise gzbuffer, gzprintf, gzwrite and adler32_combine";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *payload_b = (const Bytef *)payload;
    uLong ad_first = adler32(0UL, payload_b, (uInt)(payload_len / 2));
    uLong ad_second = adler32(0UL, payload_b + (payload_len / 2), (uInt)(payload_len - (payload_len / 2)));
    uLong ad_combined = adler32_combine(ad_first, ad_second, (off_t)(payload_len - (payload_len / 2)));

    // step 2: Configure - open gzFile and set internal buffer
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_buf = gzbuffer(gf, 8192);

    // step 3: Operate - write a header and the payload to the gzFile
    int rc_hdr = gzprintf(gf, "ZLIB-SEQ-HEADER\n");
    int rc_write = gzwrite(gf, (voidpc)payload, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));

    // step 4: Validate and cleanup - compute full checksum, close file, and tidy up
    uLong ad_full = adler32(0UL, payload_b, (uInt)payload_len);
    uLong ad_check_xor = ad_full ^ ad_combined; /* simple non-branching validation value */
    int rc_close = gzclose(gf);

    (void)ad_first;
    (void)ad_second;
    (void)ad_combined;
    (void)rc_buf;
    (void)rc_hdr;
    (void)rc_write;
    (void)ad_full;
    (void)ad_check_xor;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}