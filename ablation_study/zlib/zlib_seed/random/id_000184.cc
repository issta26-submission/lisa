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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and sources
    const char src_c[] = "zlib API sequence payload: compress2 -> deflateBound -> inflateSetDictionary -> gzclearerr";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    z_stream boundStrm;
    memset(&boundStrm, 0, sizeof(z_stream));
    uLong bound = deflateBound(&boundStrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress using compress2
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize inflate stream and set a dictionary
    const char dict_c[] = "preset_dictionary_example";
    const Bytef *dict = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_setdict = inflateSetDictionary(&istrm, dict, dictLen);

    // step 4: Operate (write compressed bytes to a gz file), validate/clear errors and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclearerr(gz);
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);

    (void)rc_comp;
    (void)bound;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}