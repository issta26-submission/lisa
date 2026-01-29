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
//<ID> 788
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[4] = {0.5, 1.5, 2.5, 3.5};
    cJSON *values_array = cJSON_CreateDoubleArray(numbers, 4);
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddNumberToObject(root, "count", 4.0);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON_AddBoolToObject(root, "equal", equal);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int parsed_size = cJSON_GetArraySize(parsed_values);
    cJSON *first_item = cJSON_GetArrayItem(parsed_values, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    double subset_nums[2] = { first_value, first_value + 1.0 };
    cJSON *subset = cJSON_CreateDoubleArray(subset_nums, 2);
    cJSON_AddItemToObject(parsed, "subset", subset);
    (void)parsed_size;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}