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
//<ID> 2411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];

    // step 3: Operate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *data_array = cJSON_GetObjectItem(root, "data");
    cJSON *first_item = cJSON_GetArrayItem(data_array, 0);
    double first_num = cJSON_GetNumberValue(first_item);
    buffer[1] = (char)(int)first_num;
    cJSON *dup_data_array = cJSON_GetObjectItem(dup_root, "data");
    cJSON *dup_first = cJSON_GetArrayItem(dup_data_array, 1);
    char *dup_first_str = cJSON_GetStringValue(dup_first);
    buffer[2] = dup_first_str[0];

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(dup_root);
    buffer[3] = printed2[0];
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}