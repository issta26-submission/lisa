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
//<ID> 1453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[1,2],\"obj\":{\"val\":3}}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);

    // step 2: Configure
    cJSON *new_number = cJSON_CreateNumber(first_val + 10.5);
    cJSON_AddItemToArray(arr, new_number);
    cJSON *arr_reference = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "arr_ref", arr_reference);

    // step 3: Operate & Validate
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "arr_ref");
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    double added_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    (void)has_ref;
    (void)printed_ok;
    (void)added_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}