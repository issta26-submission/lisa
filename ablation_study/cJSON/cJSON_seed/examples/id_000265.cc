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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *ver = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    char *tmp = (char*)cJSON_malloc(32);
    memset(tmp, 0, 32);
    tmp[0] = 't'; tmp[1] = 'e'; tmp[2] = 'm'; tmp[3] = 'p'; tmp[4] = '_'; tmp[5] = 'v'; tmp[6] = '1'; tmp[7] = '\0';
    cJSON *tmp_item = cJSON_CreateString(tmp);
    cJSON_AddItemToObject(root, "temp", tmp_item);
    cJSON_free(tmp);
    cJSON *raw_item = cJSON_CreateRaw("RAW_CONTENT");
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", ver_item);

    // step 3: Operate & Validate
    int sz = cJSON_GetArraySize(arr);
    (void)sz;
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    cJSON_bool third_is_raw = cJSON_IsRaw(third);
    cJSON_AddItemToObject(root, "replaced_ok", cJSON_CreateBool(replaced_ok));
    cJSON_AddItemToObject(root, "third_is_raw", cJSON_CreateBool(third_is_raw));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(99.9));

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}