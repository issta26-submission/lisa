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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Step 1: Declarations
    z_stream inf_strm;
    gzFile file_gz = Z_NULL;
    const char *temp_filename = "zlib_api_test_sequence_file.gz";
    char gz_write_buf[128];
    char gz_read_buf[128];
    char small_gz_read_buf[2]; // Buffer for gzgets edge case (1 char + null terminator)
    Bytef uncompress_src_buf[64];
    Bytef uncompress_dest_buf[128];
    uLongf uncompress_actual_dest_len;
    uLong uncompress_src_len_val;
    int ret;
    char *gets_result;
    unsigned int bytes_read_gz;

    // Step 2: Setup (Initialization)
    memset(&inf_strm, 0, sizeof(z_stream));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    ret = inflateInit_(&inf_strm, ZLIB_VERSION, (int)sizeof(z_stream));

    // Prepare data for gzwrite (needed to create a file for gzread/gzgets)
    strcpy(gz_write_buf, "This is the first line for gzgets.\nAnd this is the second line for gzread.\n");
    
    // Prepare data for uncompress. This is raw data, uncompress will likely return Z_DATA_ERROR,
    // which is a valid test for library robustness.
    strcpy((char*)uncompress_src_buf, "Simple raw data for uncompress test.");
    uncompress_src_len_val = strlen((char*)uncompress_src_buf);
    uncompress_actual_dest_len = sizeof(uncompress_dest_buf);
    memset(uncompress_dest_buf, 0, sizeof(uncompress_dest_buf));

    // Step 3: GZ File Operations (Write to create file)
    file_gz = gzopen(temp_filename, "wb");
    gzwrite(file_gz, gz_write_buf, strlen(gz_write_buf));
    ret = gzclose(file_gz);

    // Step 4: Core Operations (GZ File Read & Stream manipulation)
    file_gz = gzopen(temp_filename, "rb"); // Re-open for reading
    
    // Read the first line using gzgets
    gets_result = gzgets(file_gz, gz_read_buf, sizeof(gz_read_buf));
    
    // Clear any potential error flags after gzgets (even if no error occurred)
    gzclearerr(file_gz);

    // Read some more bytes using gzread
    bytes_read_gz = gzread(file_gz, gz_read_buf, 20);

    // Call inflateUndermine with a valid byte count
    ret = inflateUndermine(&inf_strm, 5);

    // Edge case: gzgets with a very small buffer (e.g., 2 bytes for 1 char + null terminator)
    // This will read just one character (the 'A' from "And...") and null-terminate it.
    gets_result = gzgets(file_gz, small_gz_read_buf, sizeof(small_gz_read_buf));

    // Step 5: Core Operations (Uncompress & Edge Cases)
    // Perform uncompress operation with raw data (expecting Z_DATA_ERROR)
    ret = uncompress(uncompress_dest_buf, &uncompress_actual_dest_len, uncompress_src_buf, uncompress_src_len_val);

    // Edge case: uncompress with zero source length
    uLongf uncompress_zero_src_dest_len = sizeof(uncompress_dest_buf);
    ret = uncompress(uncompress_dest_buf, &uncompress_zero_src_dest_len, uncompress_src_buf, 0);

    // Edge case: inflateUndermine with zero bytes
    ret = inflateUndermine(&inf_strm, 0);

    // Edge case: gzread with zero length
    bytes_read_gz = gzread(file_gz, gz_read_buf, 0);

    // Step 6: Cleanup
    ret = gzclose(file_gz); // Close the read file
    ret = inflateEnd(&inf_strm);
    remove(temp_filename); // Clean up the temporary file

    printf("API sequence test completed successfully\n");

    return 66;
}