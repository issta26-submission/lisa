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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress the source payload
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> gzfile roundtrip";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Uncompress into a decompression buffer (Operate)
    uLongf decompAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    uLongf decompLen = decompAlloc;
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);

    // step 3: Write decompressed data to a gzip file, rewind, read back (Validate)
    const char *version = zlibVersion();
    (void)version; // version observed as part of the sequence

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    int rc_gz_rewind = gzrewind(gz);
    Bytef *readBuf = (Bytef *)malloc((size_t)(decompLen + 8));
    memset(readBuf, 0, (size_t)(decompLen + 8));
    int rc_gz_read = gzread(gz, (voidp)readBuf, (unsigned int)decompLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    free(compBuf);
    free(decompBuf);
    free(readBuf);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)bound;
    (void)sourceLen;
    (void)compLen;
    (void)decompAlloc;
    (void)decompLen;

    // API sequence test completed successfully
    return 66;
}