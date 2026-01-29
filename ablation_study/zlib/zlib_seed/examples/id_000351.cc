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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and version query
    const char payload[] = "zlib API sequence payload: testing deflate with dictionary, pending, adler combine, and gzerror";
    const Bytef *inBuf = (const Bytef *)payload;
    uLong inLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize and configure deflate with a preset dictionary
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    Bytef dict[] = "preset_dictionary_for_deflate";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_set_dict = deflateSetDictionary(&dstrm, dict, dictLen);

    // step 3: Operate: compress input, query pending bits, compute adler checksums, write compressed data to a gzip file and get gzerror
    uLong outBound = deflateBound(&dstrm, inLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_in = (Bytef *)inBuf;
    dstrm.avail_in = (uInt)inLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_deflate = deflate(&dstrm, 0);
    unsigned int pending;
    int bits;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    uLong ad1 = adler32(0UL, inBuf, (uInt)inLen);
    uLong ad2 = adler32(0UL, dict, dictLen);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)dictLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gz, outBuf, (unsigned int)dstrm.total_out);
    int gz_errnum;
    const char *gz_errstr = gzerror(gz, &gz_errnum);
    int rc_gzclose = gzclose(gz);

    // step 4: Validate (by retaining return codes) and cleanup
    int rc_def_end = deflateEnd(&dstrm);
    free(outBuf);

    (void)version;
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_deflate;
    (void)rc_pending;
    (void)pending;
    (void)bits;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)rc_gzwrite;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_gzclose;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}