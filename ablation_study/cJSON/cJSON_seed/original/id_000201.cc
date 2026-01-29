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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, str1);
    cJSON_AddItemToArray(array, num2);

    // step 2: Configure
    char *buffered = cJSON_PrintBuffered(root, 128, 1);
    void *tmp_mem = cJSON_malloc(64);
    memset(tmp_mem, 0, 64);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(array);
    cJSON *second_item = cJSON_GetArrayItem(array, 1);
    cJSON *detached = cJSON_DetachItemViaPointer(array, second_item);
    int size_after = cJSON_GetArraySize(array);
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    cJSON_bool detached_is_string = cJSON_IsString(detached);
    cJSON *copy_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy_root, 1);
    (void)size_before;
    (void)size_after;
    (void)first_value;
    (void)detached_is_string;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(buffered);
    cJSON_free(tmp_mem);
    cJSON_Delete(detached);
    cJSON_Delete(copy_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}