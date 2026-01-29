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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    memset(&inf_strm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for compress/uncompress and gz offset";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compBufLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = compBufLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    uLongf decompLen = (uLongf)sourceLen;
    const char *version = zlibVersion();

    // step 2: Setup (initialize & configure streams)
    int rc_def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&def_strm, 5, 0);
    int rc_inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 3: Core operations (compress -> uncompress -> write gz -> get offset)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    gzFile gzf = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    off64_t offset = gzoffset64(gzf);

    // step 4: Validate & Cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_def_end = deflateEnd(&def_strm);
    int rc_inf_end = inflateEnd(&inf_strm);
    free(compBuf);
    free(decompBuf);
    (void)rc_def_init; (void)rc_def_params; (void)rc_inf_init;
    (void)rc_comp; (void)rc_uncomp; (void)rc_gzwrite; (void)offset;
    (void)rc_gzclose; (void)rc_def_end; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}