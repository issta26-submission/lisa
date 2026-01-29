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
//<ID> 1793
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
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddStringToObject(obj, "name", "alpha");
    cJSON_AddItemToObject(root, "object", obj);

    // step 2: Configure
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, t);
    cJSON *dup = cJSON_Duplicate(obj, 1);
    cJSON_AddItemToArray(arr, dup);
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddStringToObject(replacement, "name", "beta");
    cJSON_ReplaceItemInObject(root, "object", replacement);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *obj_after = cJSON_GetObjectItem(root, "object");
    const char *second_name = cJSON_GetStringValue(cJSON_GetObjectItem(second, "name"));
    const char *after_name = cJSON_GetStringValue(cJSON_GetObjectItem(obj_after, "name"));
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = second_name ? second_name[0] : 'Z';
    buffer[2] = after_name ? after_name[0] : 'Y';
    buffer[3] = (char)('0' + (size % 10));
    (void)first;
    (void)second_name;
    (void)after_name;
    (void)size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}