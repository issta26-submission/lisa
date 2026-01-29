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
//<ID> 968
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and checksum computation
    const char payload[] = "zlib API sequence payload exercising adler32_combine, gzbuffer, gzprintf and gzwrite";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src = (const Bytef *)payload;
    uLong len1 = payload_len / 2;
    uLong len2 = payload_len - len1;
    uLong ad1 = adler32(0UL, src, (uInt)len1);
    uLong ad2 = adler32(0UL, src + len1, (uInt)len2);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)len2);

    // step 2: Open and configure gzFile
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 4096);
    int rc_gzprintf_hdr = gzprintf(gf, "BEGIN_PAYLOAD\n");

    // step 3: Write payload content
    int rc_gzwrite = gzwrite(gf, (voidpc)src, (unsigned int)payload_len);
    int rc_gzprintf_ftr = gzprintf(gf, "\nEND_PAYLOAD\n");

    // step 4: Validate (compute full checksum) and cleanup resources
    uLong ad_full = adler32(0UL, src, (uInt)payload_len);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);

    (void)len1;
    (void)len2;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)ad_full;
    (void)rc_gzbuffer;
    (void)rc_gzprintf_hdr;
    (void)rc_gzwrite;
    (void)rc_gzprintf_ftr;
    (void)rc_gzflush;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}