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
//<ID> 712
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[] = {10, 20, 30, 40};
    cJSON *ints = cJSON_CreateIntArray(nums, 4);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *label = cJSON_CreateString("int_sequence");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    char *serialized = cJSON_PrintUnformatted(root);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(serialized, strlen(serialized), &parse_end, 1);

    // step 3: Operate
    int original_count = cJSON_GetArraySize(ints);
    cJSON *parsed_ints = cJSON_GetObjectItem(parsed, "ints");
    int parsed_count = cJSON_GetArraySize(parsed_ints);
    cJSON_AddNumberToObject(root, "original_count", (double)original_count);
    cJSON_AddNumberToObject(root, "parsed_count", (double)parsed_count);

    // step 4: Validate & Cleanup
    char *label_text = cJSON_GetStringValue(label);
    cJSON *label_copy = cJSON_CreateString(label_text);
    cJSON_AddItemToObject(root, "label_copy", label_copy);
    cJSON_free(serialized);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}