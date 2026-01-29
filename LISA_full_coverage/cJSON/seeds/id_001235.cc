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
//<ID> 1235
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
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(arr, num);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "child_key", "child_value");
    cJSON_bool ref_added_array = cJSON_AddItemReferenceToArray(arr, child);
    cJSON_bool ref_added_object = cJSON_AddItemReferenceToObject(root, "ref_child", child);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buf = (char *)cJSON_malloc(printed_len + 32);
    memset(buf, 0, printed_len + 32);
    memcpy(buf, printed, printed_len);
    cJSON_Minify(buf);
    int arr_size_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 0);
    int arr_size_after = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON *retrieved_field = cJSON_GetObjectItem(first_item, "child_key");
    const char *child_value = cJSON_GetStringValue(retrieved_field);

    // step 4: Validate & Cleanup
    (void)ref_added_array;
    (void)ref_added_object;
    (void)arr_size_before;
    (void)arr_size_after;
    (void)child_value;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}