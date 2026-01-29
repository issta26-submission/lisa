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
//<ID> 620
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    int numbers[] = { 10, 20, 30, 40 };
    cJSON *ints = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *null_item = cJSON_AddNullToObject(root, "missing");
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 2: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    char *status = (char *)cJSON_malloc(256);
    memset(status, 0, 256);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_ints = cJSON_GetObjectItem(parsed, "ints");
    cJSON *first_elem = cJSON_GetArrayItem(parsed_ints, 0);
    double first_value = cJSON_GetNumberValue(first_elem);
    cJSON *parsed_missing = cJSON_GetObjectItem(parsed, "missing");
    cJSON_bool missing_was_null = cJSON_IsNull(parsed_missing);

    // step 3: Validate
    sprintf(status, "ver=%s;first=%.0f;missing_null=%d",
            version ? version : "null",
            first_value,
            (int)missing_was_null);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(parsed, "missing");
    cJSON_Delete(parsed);
    cJSON_DeleteItemFromObject(root, "ints");
    cJSON_Delete(root);
    cJSON_free(snapshot);
    cJSON_free(status);
    // API sequence test completed successfully
    return 66;
}