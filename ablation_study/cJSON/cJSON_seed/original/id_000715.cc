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
//<ID> 715
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[] = { 10, 20, 30, 40 };
    cJSON *root = cJSON_CreateObject();
    cJSON *ints = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *label = cJSON_CreateString("integer_sequence");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure (serialize and parse with length/options)
    char *serialized = cJSON_PrintUnformatted(root);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(serialized, (size_t)strlen(serialized), &parse_end, 1);

    // step 3: Operate (inspect array size and attach a string representation)
    cJSON *parsed_ints = cJSON_GetObjectItem(parsed, "ints");
    int count = cJSON_GetArraySize(parsed_ints);
    char count_buf[32];
    memset(count_buf, 0, sizeof(count_buf));
    snprintf(count_buf, sizeof(count_buf), "%d", count);
    cJSON *count_str = cJSON_CreateString(count_buf);
    cJSON_AddItemToObject(parsed, "ints_count_str", count_str);

    // step 4: Validate & Cleanup
    cJSON *parsed_dup = cJSON_Duplicate(parsed, 1);
    char *out_unformatted = cJSON_PrintUnformatted(parsed_dup);
    cJSON_free(out_unformatted);
    cJSON_free(serialized);
    cJSON_Delete(parsed_dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}