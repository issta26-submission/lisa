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
//<ID> 314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num1);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num2);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *raw_blob = cJSON_AddRawToObject(root, "raw_blob", "{\"x\":1}");
    cJSON *str_item = cJSON_AddStringToObject(root, "greeting", "hello");

    // step 3: Operate and Validate
    cJSON_bool raw_is = cJSON_IsRaw(raw_blob);
    (void)raw_is;
    cJSON_bool root_invalid = cJSON_IsInvalid(root);
    (void)root_invalid;
    char *greeting = cJSON_GetStringValue(str_item);
    (void)greeting;
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