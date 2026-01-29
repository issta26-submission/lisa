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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "values");
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_blob", "{\"x\":1,\"y\":2}");

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "values_copy", dup_arr);
    char *name_val = cJSON_GetStringValue(name);
    cJSON_AddStringToObject(root, "name_copy", name_val);

    // step 3: Operate and Validate
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_bool dup_invalid = cJSON_IsInvalid(dup_arr);
    (void)raw_is_raw;
    (void)dup_invalid;
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}