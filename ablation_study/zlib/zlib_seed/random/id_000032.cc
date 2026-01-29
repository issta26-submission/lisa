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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char src_c[] = "Example source data for zlib";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char dict_c[] = "shared dictionary content";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    Bytef dest[1024];
    uLongf destLen = (uLongf)sizeof(dest);
    uLong bound = compressBound(sourceLen);
    uLongf useBound = (uLongf)(bound < (uLong)sizeof(dest) ? bound : (uLong)sizeof(dest));

    // Step 2: Setup -> Configure (compress source, init inflate, set dictionary, validate)
    int rc_compress = compress(dest, &destLen, source, sourceLen);
    const char *ver = zlibVersion();
    int rc_inflateInit = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = dest;
    strm.avail_in = (uInt)(destLen <= 0xFFFFFFFFu ? (uInt)destLen : 0xFFFFFFFFu);
    strm.next_out = dest;
    strm.avail_out = (uInt)sizeof(dest);
    int rc_setdict = inflateSetDictionary(&strm, dictionary, dictLen);
    int rc_validate = inflateValidate(&strm, 1);

    // Step 3: Operate -> Validate (write results to gzip file and query error)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int wrote = gzwrite(gz, dest, (unsigned int)(useBound <= (uLongf)0xFFFFFFFFu ? (unsigned int)useBound : 0xFFFFFFFFu));
    int gz_errnum = 0;
    const char *errstr = gzerror(gz, &gz_errnum);
    gzprintf(gz, "ver=%s rc_compress=%d destLen=%lu rc_inflateInit=%d rc_setdict=%d rc_validate=%d gz_errnum=%d err=%s\n",
             ver, rc_compress, (unsigned long)destLen, rc_inflateInit, rc_setdict, rc_validate, gz_errnum, errstr);

    // Step 4: Cleanup
    inflateEnd(&strm);
    int rc_close = gzclose(gz);
    (void)bound; (void)useBound; (void)wrote; (void)rc_close; (void)rc_compress; (void)rc_inflateInit; (void)rc_setdict; (void)rc_validate; (void)gz_errnum; (void)errstr;

    // API sequence test completed successfully
    return 66;
}