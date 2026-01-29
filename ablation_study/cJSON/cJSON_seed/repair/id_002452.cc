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
//<ID> 2452
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    buffer[1] = (char)0;

    // step 3: Operate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    buffer[1] = (char)(int)second_val;
    cJSON_bool has_count = cJSON_HasObjectItem(root, "count");
    buffer[2] = (char)(int)has_count;
    cJSON *ref = cJSON_CreateArrayReference(second_item);
    cJSON_AddItemToObject(root, "second_ref", ref);
    cJSON *third_item = cJSON_GetArrayItem(arr, 2);
    const char *third_str = cJSON_GetStringValue(third_item);
    buffer[3] = third_str && third_str[0] ? third_str[0] : '\0';

    // step 4: Validate and Cleanup
    double count_val = cJSON_GetNumberValue(count_item);
    buffer[4] = (char)(int)count_val;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}