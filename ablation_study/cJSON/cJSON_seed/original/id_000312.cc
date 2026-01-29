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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "data");
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *label_item = cJSON_AddStringToObject(root, "label", "example");
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_blob", "{\"x\":1}");

    // step 2: Configure
    cJSON *placeholder = cJSON_CreateNumber(0.0);
    cJSON_AddItemToArray(arr, placeholder);
    const char *label_str = cJSON_GetStringValue(label_item);
    double derived_num = (double)label_str[0];
    cJSON *derived = cJSON_CreateNumber(derived_num);
    cJSON_ReplaceItemInArray(arr, 2, derived);

    // step 3: Operate and Validate
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_bool label_is_invalid = cJSON_IsInvalid(label_item);
    (void)raw_is_raw;
    (void)label_is_invalid;
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}