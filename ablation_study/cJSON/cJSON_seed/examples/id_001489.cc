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
//<ID> 1489
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    void *buffer = cJSON_malloc(256);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    double first_value = cJSON_GetNumberValue(n1);
    (void)arr_is_array;
    (void)first_value;

    // step 3: Operate & Validate
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    double item0_v = cJSON_GetNumberValue(item0);
    cJSON *dup_values = cJSON_GetObjectItem(duplicate, "values");
    cJSON_bool dup_is_array = cJSON_IsArray(dup_values);
    (void)item0_v;
    (void)dup_is_array;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}