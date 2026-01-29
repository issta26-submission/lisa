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
//<ID> 238
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
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    int size_before = cJSON_GetArraySize(arr);
    char *printed = cJSON_Print(root);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 1);
    int size_after = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_num = cJSON_GetNumberValue(first);
    const char *maybe_str = cJSON_GetStringValue(first);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    (void)flag_is_bool;
    (void)size_before;
    (void)size_after;
    (void)first_num;
    (void)maybe_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}