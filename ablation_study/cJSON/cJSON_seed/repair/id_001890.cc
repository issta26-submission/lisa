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
//<ID> 1890
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
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *num1 = cJSON_CreateNumber(3.5);
    cJSON *str2 = cJSON_CreateString("hello");
    cJSON_InsertItemInArray(arr, 0, num0);
    cJSON_InsertItemInArray(arr, 1, num1);
    cJSON_InsertItemInArray(arr, 2, str2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("1.0"));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *got_item1 = cJSON_GetArrayItem(arr, 1);
    cJSON *got_item2 = cJSON_GetArrayItem(arr, 2);
    cJSON *newmeta = cJSON_CreateObject();
    cJSON_AddItemToObject(newmeta, "replaced", cJSON_CreateTrue());
    cJSON_ReplaceItemInObject(root, "meta", newmeta);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    double num_val = cJSON_GetNumberValue(got_item1);
    const char *str_val = cJSON_GetStringValue(got_item2);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed[0];
    int digit = ((int)num_val % 10 + 10) % 10;
    buffer[1] = (char)('0' + digit);
    buffer[2] = str_val[0];
    buffer[3] = (char)('0' + (cJSON_GetArraySize(arr) % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}