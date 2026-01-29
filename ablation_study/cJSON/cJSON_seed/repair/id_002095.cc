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
//<ID> 2095
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
    cJSON *str_a = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, str_a);
    cJSON *str_b = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(arr, 0, str_b);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "title", "demo_root");
    cJSON_AddItemToObject(root, "meta", meta);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *dump_copy = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "dump", dump_copy);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    const char *first_val = cJSON_GetStringValue(first);
    const char *second_val = cJSON_GetStringValue(second);
    cJSON_AddStringToObject(root, "first_item", first_val);
    cJSON_AddStringToObject(root, "second_item", second_val);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}