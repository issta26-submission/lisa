#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 695
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printbuf = (char *)cJSON_malloc(128);
    memset(printbuf, 0, 128);
    const char *endptr1 = NULL;
    const char *endptr2 = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed_from_buffer = NULL;
    cJSON *parsed_from_literal = NULL;
    cJSON *s_item = NULL;
    cJSON_bool printed_ok = 0;
    cJSON_bool s_is_string = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "name", "tester");
    cJSON_AddBoolToObject(root, "initial_flag", 0);
    cJSON_AddNumberToObject(root, "count", 7.0);

    // step 3: Operate / Validate
    printed_ok = cJSON_PrintPreallocated(root, printbuf, 128, 1);
    memcpy(scratch, printbuf ? printbuf : "", 127);
    scratch[127] = '\0';
    parsed_from_buffer = cJSON_ParseWithLengthOpts(printbuf, 128, &endptr1, 1);
    const char *literal = "{\"s\":\"hello\",\"n\":42}";
    parsed_from_literal = cJSON_ParseWithOpts(literal, &endptr2, 1);
    s_item = cJSON_GetObjectItem(parsed_from_literal, "s");
    s_is_string = cJSON_IsString(s_item);
    cJSON_AddBoolToObject(root, "printed_ok", printed_ok);
    cJSON_AddBoolToObject(root, "s_is_string", s_is_string);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printbuf);
    cJSON_Delete(parsed_from_buffer);
    cJSON_Delete(parsed_from_literal);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}