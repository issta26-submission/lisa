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
//<ID> 247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));

    /* small fixed-size input so stack buffers are safely large enough */
    unsigned char rawData[128];
    memset(rawData, 'R', sizeof(rawData));
    uLong rawLen = (uLong)sizeof(rawData);

    /* initialize deflate state */
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));

    /* set a preset dictionary */
    Bytef presetDict[32];
    memset(presetDict, 'D', sizeof(presetDict));
    deflateSetDictionary(&strm, presetDict, (uInt)sizeof(presetDict));

    /* allocate fixed stack buffers that comfortably exceed compressBound(rawLen) */
    Bytef compBuf[512];
    memset(compBuf, 0, sizeof(compBuf));
    strm.next_in = (Bytef *)rawData;
    strm.avail_in = (uInt)rawLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)sizeof(compBuf);

    /* compress the data and finish the stream */
    deflate(&strm, Z_FINISH);

    /* retrieve any dictionary information and reset keep state */
    Bytef retrievedDict[64];
    uInt retrievedLen = (uInt)sizeof(retrievedDict);
    deflateGetDictionary(&strm, retrievedDict, &retrievedLen);
    deflateResetKeep(&strm);

    /* prepare output buffer and attempt to uncompress the produced compressed data */
    Bytef outBuf[128];
    memset(outBuf, 0, sizeof(outBuf));
    uLongf outLen = (uLongf)sizeof(outBuf);
    uncompress(outBuf, &outLen, compBuf, (uLong)strm.total_out);

    /* write decompressed data and metadata into a gzip file */
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)outLen);
    gzseek(gz, (off_t)0, 0);
    gzputs(gz, "METADATA:retrieved_dict_size=64\n");
    gzclose(gz);

    /* cleanup */
    deflateEnd(&strm);

    return 66;
}