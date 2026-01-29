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
//<ID> 961
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and checksum computation (Initialize)
    const char payload[] = "zlib API sequence payload exercising gzbuffer, gzprintf, gzwrite and adler32_combine";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong ad1 = adler32(0UL, src_buf, (uInt)payload_len);
    uLong ad2 = adler32(1UL, src_buf, (uInt)payload_len);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)payload_len);

    // step 2: Open and configure gzFile (Configure)
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 8192);

    // step 3: Write payload and metadata using gzwrite and gzprintf (Operate / Validate)
    int rc_gzwrite_payload = gzwrite(gf, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzprintf = gzprintf(gf, "ADLER_COMBINED:\n");
    int rc_gzwrite_adler = gzwrite(gf, (voidpc)&ad_combined, (unsigned int)sizeof(ad_combined));

    // step 4: Flush and close the gzFile (Cleanup)
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);

    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)rc_gzbuffer;
    (void)rc_gzwrite_payload;
    (void)rc_gzprintf;
    (void)rc_gzwrite_adler;
    (void)rc_gzflush;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}