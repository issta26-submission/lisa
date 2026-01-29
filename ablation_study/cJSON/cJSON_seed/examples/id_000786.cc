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
//<ID> 786
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double numbers[4] = {0.5, 1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *values_array = cJSON_CreateDoubleArray(numbers, 4);
    cJSON *label = cJSON_CreateString("measurements");
    cJSON *count = cJSON_CreateNumber(4.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", cJSON_CreateString("sensor-A"));
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddItemToObject(root, "count", count);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *snapshot = cJSON_PrintUnformatted(dup);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    int parsed_size = cJSON_GetArraySize(cJSON_GetObjectItem(parsed, "values"));
    cJSON *first_item = cJSON_GetArrayItem(cJSON_GetObjectItem(parsed, "values"), 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *augmented = cJSON_CreateNumber(first_value + 10.0);
    cJSON_AddItemToObject(parsed, "first_plus_10", augmented);
    (void)equal;
    (void)parsed_size;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}