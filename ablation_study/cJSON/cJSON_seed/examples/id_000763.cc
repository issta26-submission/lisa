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
//<ID> 763
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    double numbers[] = {1.1, 2.2, 3.3};
    cJSON *num_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *nullable = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(child, "numbers", num_array);
    cJSON_AddNumberToObject(child, "version", 2.5);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "nullable", nullable);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_nullable = cJSON_GetObjectItem(parsed, "nullable");
    cJSON_bool is_null = cJSON_IsNull(parsed_nullable);
    (void)is_null;
    cJSON *parsed_child = cJSON_GetObjectItem(parsed, "child");
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed_child, "numbers");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(parsed_numbers, 0));
    (void)parsed_count;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}