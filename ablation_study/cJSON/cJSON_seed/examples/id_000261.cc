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
//<ID> 261
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
    cJSON *raw = cJSON_CreateRaw("RAW_DATA");
    cJSON *n2 = cJSON_CreateNumber(20.0);
    const char *version = cJSON_Version();
    char *mem = (char*)cJSON_malloc(8);
    memset(mem, 0, 8);
    mem[0] = version[0];
    mem[1] = '\0';
    cJSON *ver_ref = cJSON_CreateStringReference((const char *)mem);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "version_ref", ver_ref);

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_raw = cJSON_IsRaw(second);
    cJSON *second_is_raw_item = cJSON_CreateBool(second_is_raw);
    cJSON_AddItemToObject(root, "second_is_raw", second_is_raw_item);
    cJSON *replacement = cJSON_CreateString("replaced_raw");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *replaced_flag = cJSON_CreateBool(replaced);
    cJSON_AddItemToObject(root, "replaced_flag", replaced_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(mem);
    return 66; // API sequence test completed successfully
}