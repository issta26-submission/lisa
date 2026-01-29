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
//<ID> 303
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
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *raw_blob = cJSON_CreateRaw("{\"meta\":true}");
    cJSON *label = cJSON_CreateString("sample");

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "blob", raw_blob);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_null = cJSON_IsNull(second);
    double second_value = cJSON_GetNumberValue(second);
    cJSON *extracted = cJSON_CreateNumber(second_value);
    cJSON_AddItemToObject(root, "extracted", extracted);
    cJSON_bool has_blob = cJSON_HasObjectItem(root, "blob");
    cJSON *has_blob_item = cJSON_CreateBool(has_blob);
    cJSON_AddItemToObject(root, "has_blob", has_blob_item);
    void *buffer = cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, (char *)buffer, 512, (cJSON_bool)0);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}