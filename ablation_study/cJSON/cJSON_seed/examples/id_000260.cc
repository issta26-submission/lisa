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
//<ID> 260
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
    cJSON *n1 = cJSON_CreateNumber(3.14159);
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON *n2 = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "items", arr);
    const char *ver = cJSON_Version();
    cJSON *ver_str = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", ver_str);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    cJSON_free(tmp_buf);

    // step 3: Operate & Validate
    cJSON_bool raw_before = cJSON_IsRaw(raw);
    cJSON *raw_flag = cJSON_CreateBool(raw_before);
    cJSON_AddItemToObject(root, "raw_was", raw_flag);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *first_copy = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *size_val = cJSON_CreateNumber((double)cJSON_GetArraySize(arr));
    cJSON_AddItemToObject(root, "count", size_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}