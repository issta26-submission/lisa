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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char src_c[] = "Example dictionary data for zlib";
    const Bytef *dictionary = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    const char *ver = zlibVersion();
    int rc_inflateInit = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    int rc_setdict;
    int rc_validate;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_errnum;
    const char *gz_errstr;

    // step 2: Configure -> Operate
    rc_setdict = inflateSetDictionary(&strm, dictionary, (uInt)sourceLen);
    rc_validate = inflateValidate(&strm, 1);
    gzprintf(gz, "compressBound=%lu inflateInit=%d setDictionary=%d validate=%d\n", (unsigned long)bound, rc_inflateInit, rc_setdict, rc_validate);

    // step 3: Operate -> Validate
    gz_errstr = gzerror(gz, &gz_errnum);
    gzprintf(gz, "gzerror=%s errnum=%d\n", gz_errstr ? gz_errstr : "NULL", gz_errnum);

    // step 4: Cleanup
    inflateEnd(&strm);
    int rc_close = gzclose(gz);
    (void)rc_close;
    (void)gz_errnum;
    (void)rc_validate;
    (void)rc_setdict;
    (void)rc_inflateInit;
    // API sequence test completed successfully
    return 66;
}