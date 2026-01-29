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
//<ID> 621
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    int numbers[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "values", int_array);
    cJSON *null_item = cJSON_AddNullToObject(root, "missing");
    cJSON_bool null_is_null = cJSON_IsNull(null_item);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "sum", 100.0);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snapshot_len = strlen(snapshot) + 1;
    char *snapshot_copy = (char *)cJSON_malloc(snapshot_len);
    memcpy(snapshot_copy, snapshot, snapshot_len);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int parsed_count = cJSON_GetArraySize(parsed_values);
    cJSON *first_val_item = cJSON_GetArrayItem(parsed_values, 0);
    double first_value = cJSON_GetNumberValue(first_val_item);
    cJSON_DeleteItemFromObject(root, "sum");
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;first=%.0f;missing_is_null=%d",
            version ? version : "null",
            parsed_count,
            first_value,
            (int)null_is_null);

    // step 4: Cleanup
    cJSON_free(snapshot_copy);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}