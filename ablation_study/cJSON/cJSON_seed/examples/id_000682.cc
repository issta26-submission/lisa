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
//<ID> 682
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
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON *true_flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, true_flag);
    cJSON_AddItemToObject(root, "data", array);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_array = cJSON_GetObjectItem(parsed, "data");
    int array_size = cJSON_GetArraySize(parsed_array);
    cJSON *first_item = cJSON_GetArrayItem(parsed_array, 0);
    cJSON *second_item = cJSON_GetArrayItem(parsed_array, 1);
    cJSON_bool first_equals_second = cJSON_Compare(first_item, second_item, 0);
    cJSON *first_dup = cJSON_Duplicate(first_item, 1);
    cJSON_bool dup_equal = cJSON_Compare(first_item, first_dup, 1);
    (void)version;
    (void)array_size;
    (void)first_equals_second;
    (void)dup_equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(first_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}