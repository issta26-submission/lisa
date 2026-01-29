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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defStrm;
    z_stream infStrm;
    memset(&defStrm, 0, sizeof(z_stream));
    memset(&infStrm, 0, sizeof(z_stream));
    const char dict_c[] = "example dictionary data for zlib";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char payload_c[] = "payload data to compress and record with gzip";
    const Bytef *payload = (const Bytef *)payload_c;
    uLong payloadLen = (uLong)(sizeof(payload_c) - 1);
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate/inflate and set dictionaries)
    int rc_deflateInit = deflateInit2_(&defStrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    int rc_defSetDict = deflateSetDictionary(&defStrm, dictionary, dictLen);
    int rc_inflateInit = inflateInit2_(&infStrm, 15, version, (int)sizeof(z_stream));
    int rc_inflateSetDict = inflateSetDictionary(&infStrm, dictionary, dictLen);

    // step 3: Operate -> Validate (prepare buffers, validate inflate, write gzip info and query offset)
    uLong bound = compressBound(payloadLen);
    uInt outBufSize = (uInt)(bound + 16);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufSize);
    defStrm.next_in = (Bytef *)payload;
    defStrm.avail_in = (uInt)payloadLen;
    defStrm.next_out = outBuf;
    defStrm.avail_out = outBufSize;
    int rc_validate = inflateValidate(&infStrm, 1);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzprintf1 = gzprintf(gz, "zlib_ver=%s dictLen=%u payloadLen=%lu defInit=%d defSetDict=%d infInit=%d infSetDict=%d validate=%d\n",
                                version, (unsigned)dictLen, (unsigned long)payloadLen, rc_deflateInit, rc_defSetDict, rc_inflateInit, rc_inflateSetDict, rc_validate);
    off64_t offset64 = gzoffset64(gz);
    int rc_gzprintf2 = gzprintf(gz, "gz_offset64=%lld\n", (long long)offset64);

    // step 4: Cleanup
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    gzclose(gz);
    free(outBuf);
    (void)rc_deflateInit; (void)rc_defSetDict; (void)rc_inflateInit; (void)rc_inflateSetDict; (void)rc_validate;
    (void)rc_gzprintf1; (void)rc_gzprintf2; (void)offset64;

    // API sequence test completed successfully
    return 66;
}