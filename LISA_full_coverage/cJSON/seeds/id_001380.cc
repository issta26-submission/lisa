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
//<ID> 1380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "[\"alpha\", \"beta\", \"gamma\"] trailing";
    const char *parse_end = NULL;
    cJSON *parsed_array = cJSON_ParseWithOpts(json, &parse_end, 0);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *first_item = cJSON_GetArrayItem(parsed_array, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    cJSON *first_copy = cJSON_CreateString(first_value);
    cJSON_AddItemToObject(root, "first", first_copy);
    cJSON *true_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_flag);
    cJSON *replacement = cJSON_CreateString("replaced_beta");

    // step 3: Operate
    cJSON_ReplaceItemInArray(parsed_array, 1, replacement);
    cJSON *array_dup = cJSON_Duplicate(parsed_array, 1);
    cJSON_AddItemToObject(root, "array_copy", array_dup);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(parsed_array);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}