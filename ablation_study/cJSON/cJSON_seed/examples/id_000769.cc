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
//<ID> 769
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON *values_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *maybe_null = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddItemToObject(root, "maybe", maybe_null);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_maybe = cJSON_GetObjectItem(parsed, "maybe");
    cJSON_bool parsed_is_null = cJSON_IsNull(parsed_maybe);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int parsed_size = cJSON_GetArraySize(parsed_values);
    cJSON *first_item = cJSON_GetArrayItem(parsed_values, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    (void)parsed_is_null;
    (void)parsed_size;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}