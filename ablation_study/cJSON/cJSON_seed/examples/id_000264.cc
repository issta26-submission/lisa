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
//<ID> 264
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
    cJSON *raw1 = cJSON_CreateRaw("{\"raw\":1}");
    cJSON *str1 = cJSON_CreateString("init_string");
    cJSON *num1 = cJSON_CreateNumber(7.5);

    // step 2: Configure
    cJSON_AddItemToArray(arr, raw1);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "items", arr);
    const char *ver = cJSON_Version();
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(ver));
    char *tmp = (char *)cJSON_malloc(4);
    tmp[0] = '1'; tmp[1] = '\0';
    cJSON_AddItemToObject(root, "malloc_flag", cJSON_CreateString(tmp));

    // step 3: Operate & Validate
    cJSON *rep = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, rep);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_raw = cJSON_IsRaw(first);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_raw = cJSON_IsRaw(second);
    cJSON *count = cJSON_CreateNumber((double)cJSON_GetArraySize(arr));
    cJSON_AddItemToObject(root, "count", count);
    cJSON_AddItemToObject(root, "first_is_raw", cJSON_CreateNumber((double)first_is_raw));
    cJSON_AddItemToObject(root, "second_is_raw", cJSON_CreateNumber((double)second_is_raw));
    cJSON_AddItemToObject(root, "replaced_flag", cJSON_CreateNumber((double)replaced));

    // step 4: Cleanup
    cJSON_free(tmp);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}