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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Step 1: Declarations
    z_stream strm_inflate;
    gzFile file_gz = Z_NULL;
    const char *filename = "zlib_api_test_file.gz";

    Bytef source_buf[100];
    Bytef dest_buf[200];
    uLongf dest_len;
    uLong source_len;

    uLong adler_val;
    int ret;
    off64_t file_pos_64;
    unsigned long codes_used;

    // Step 2: Setup and z_stream Initialization
    memset(source_buf, 'A', sizeof(source_buf) - 1);
    source_buf[sizeof(source_buf) - 1] = '\n'; // Add newline for gzprintf readability
    source_len = sizeof(source_buf);
    dest_len = sizeof(dest_buf);

    memset(&strm_inflate, 0, sizeof(z_stream));
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;
    // Initialize an inflate stream for inflateCodesUsed and inflateResetKeep
    ret = inflateInit_(&strm_inflate, ZLIB_VERSION, (int)sizeof(z_stream));

    // Initialize adler32 checksum
    adler_val = adler32(0L, Z_NULL, 0);

    // Step 3: compress and adler32 Operations
    ret = compress(dest_buf, &dest_len, source_buf, source_len);

    // Calculate adler32 on the source data
    adler_val = adler32(adler_val, source_buf, (uInt)source_len);

    // Edge Case: adler32 with a NULL buffer and non-zero length
    // Zlib's adler32 handles this by returning 1L (ADLER_BASE) if buf is Z_NULL and len > 0.
    adler_val = adler32(adler_val, Z_NULL, 15);

    // Step 4: GZ File Operations (Write, Tell, Offset)
    file_gz = gzopen(filename, "wb");
    ret = gzprintf(file_gz, "This is a test string written to gzfile. Compressed data length: %lu.\n", dest_len);
    
    // Get current file position and uncompressed offset
    file_pos_64 = gztell64(file_gz);
    file_pos_64 = gzoffset64(file_gz);

    // Edge Case: gzprintf with an empty format string
    ret = gzprintf(file_gz, "");

    ret = gzclose(file_gz);

    // Step 5: z_stream Operations (Inflate-related)
    // Get codes used by the inflate stream (should be 0 as no inflate operation yet)
    codes_used = inflateCodesUsed(&strm_inflate);

    // Reset the inflate stream, preserving dictionary (which is empty here)
    ret = inflateResetKeep(&strm_inflate);

    // Edge Case: Call inflateResetKeep again on the already reset stream
    // This demonstrates robustness against redundant calls.
    ret = inflateResetKeep(&strm_inflate);

    // Step 6: Cleanup
    ret = inflateEnd(&strm_inflate); // Clean up the inflate stream
    remove(filename); // Remove the temporary file

    printf("API sequence test completed successfully\n");

    return 66;
}