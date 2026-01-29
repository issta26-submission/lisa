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
//<ID> 1102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_flag = cJSON_CreateTrue();
    cJSON *num1 = cJSON_CreateNumber(12.5);
    cJSON *num2 = cJSON_CreateNumber(7.75);

    // step 2: Configure
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "enabled", true_flag);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddNumberToObject(stats, "count", 2.0);
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate & Validate
    cJSON_bool is_array = cJSON_IsArray(arr);
    cJSON *count_item = cJSON_GetObjectItem(stats, "count");
    double count_value = cJSON_GetNumberValue(count_item);
    double doubled = count_value * 2.0;
    cJSON_AddNumberToObject(stats, "double_count", doubled);
    int array_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObject(root, "size", size_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    (void)is_array;
    (void)count_value;
    (void)doubled;
    (void)array_size;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}