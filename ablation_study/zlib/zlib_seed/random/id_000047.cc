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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char dict_c[] = "example_dictionary_for_zlib_usage";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    z_stream strm_def;
    z_stream strm_inf;
    memset(&strm_def, 0, sizeof(z_stream));
    memset(&strm_inf, 0, sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Configure (initialize deflate and inflate, set dictionaries)
    int rc_deflate_init = deflateInit_(&strm_def, 6, version, (int)sizeof(z_stream));
    int rc_deflate_setdict = deflateSetDictionary(&strm_def, dictionary, dictLen);
    int rc_inflate_init = inflateInit2_(&strm_inf, 15, version, (int)sizeof(z_stream));
    int rc_inflate_setdict = inflateSetDictionary(&strm_inf, dictionary, dictLen);

    // step 3: Operate -> Validate (open gzip file, record offset, log results)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    off64_t offset = gzoffset64(gz);
    gzprintf(gz, "ver=%s deflateInit=%d deflateSetDict=%d inflateInit2=%d inflateSetDict=%d dictLen=%u offset=%lld\n",
             version,
             rc_deflate_init,
             rc_deflate_setdict,
             rc_inflate_init,
             rc_inflate_setdict,
             (unsigned int)dictLen,
             (long long)offset);

    // step 4: Cleanup
    int rc_deflate_end = deflateEnd(&strm_def);
    int rc_inflate_end = inflateEnd(&strm_inf);
    int rc_gzclose = gzclose(gz);
    (void)rc_deflate_init; (void)rc_deflate_setdict; (void)rc_inflate_init; (void)rc_inflate_setdict;
    (void)rc_deflate_end; (void)rc_inflate_end; (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}