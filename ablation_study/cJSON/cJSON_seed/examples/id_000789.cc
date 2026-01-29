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
//<ID> 789
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[4] = { 0.5, 1.5, 2.5, 3.5 };
    cJSON *data_array = cJSON_CreateDoubleArray(numbers, 4);
    cJSON *data_array_dup = cJSON_Duplicate(data_array, 1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "data", data_array);
    cJSON_AddItemToObject(root, "data_copy", data_array_dup);
    cJSON_AddNumberToObject(root, "count", 4.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_data = cJSON_GetObjectItem(parsed, "data");
    int parsed_size = cJSON_GetArraySize(parsed_data);
    cJSON *first_item = cJSON_GetArrayItem(parsed_data, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON_bool arrays_equal = cJSON_Compare(data_array, data_array_dup, 1);
    cJSON_bool objects_equal = cJSON_Compare(root, parsed, 1);
    (void)parsed_size;
    (void)first_value;
    (void)arrays_equal;
    (void)objects_equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}