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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char dict_c[] = "example preset dictionary for zlib";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char src_c[] = "payload data that benefits from a preset dictionary";
    const Bytef *payload = (const Bytef *)src_c;
    uInt payloadLen = (uInt)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate and inflate streams and attach dictionaries)
    int rc_deflateInit = deflateInit2_(&dstrm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY, version, (int)sizeof(z_stream));
    int rc_defSetDict = deflateSetDictionary(&dstrm, dictionary, dictLen);
    int rc_inflateInit = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inflateSetDict = inflateSetDictionary(&istrm, dictionary, dictLen);

    // step 3: Operate -> Validate (compress with deflate, then inflate back; record via gz)
    uLong bound = deflateBound(&dstrm, (uLong)payloadLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound + 16);
    uInt compBufSize = (uInt)(bound + 16);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = payloadLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = compBufSize;
    int rc_deflate = deflate(&dstrm, Z_FINISH);

    uInt compSize = (uInt)dstrm.total_out;
    Bytef *outBuf = (Bytef *)malloc((size_t)payloadLen + 16);
    uInt outBufSize = payloadLen + 16;
    istrm.next_in = compBuf;
    istrm.avail_in = compSize;
    istrm.next_out = outBuf;
    istrm.avail_out = outBufSize;
    int rc_inflate = inflate(&istrm, Z_FINISH);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    off64_t offset = gzoffset64(gz);
    int rc_gzprintf = gzprintf(gz, "ver=%s defInit=%d defSetDict=%d deflate=%d compSize=%u inflateInit=%d inflateSetDict=%d inflate=%d outSize=%u off=%lld\n",
                               version, rc_deflateInit, rc_defSetDict, rc_deflate, (unsigned int)compSize, rc_inflateInit, rc_inflateSetDict, rc_inflate, (unsigned int)istrm.total_out, (long long)offset);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    gzclose(gz);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}