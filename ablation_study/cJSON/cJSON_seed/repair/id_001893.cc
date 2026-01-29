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
//<ID> 1893
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 0, num);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_InsertItemInArray(arr, 1, str);
    cJSON *t = cJSON_CreateTrue();
    cJSON_InsertItemInArray(arr, 2, t);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *meta_new = cJSON_CreateObject();
    cJSON_AddItemToObject(meta_new, "version", cJSON_CreateString("1.0"));
    cJSON_ReplaceItemInObject(root, "meta", meta_new);

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    const char *second_value = cJSON_GetStringValue(second_item);
    int arr_size = cJSON_GetArraySize(arr);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (arr_size % 10));
    buffer[2] = second_value[0];
    buffer[3] = (char)('0' + (cJSON_IsObject(meta_new) % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}