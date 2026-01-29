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
//<ID> 669
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr1 = cJSON_CreateArray();
    cJSON_AddItemToArray(arr1, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(arr1, cJSON_CreateNumber(42.0));
    cJSON *arr_in_root = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemReferenceToArray(arr_in_root, arr1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "description", "array reference example");
    cJSON_AddNumberToObject(meta, "len", (double)cJSON_GetArraySize(arr1));

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr1, 0);
    cJSON_bool first_is_str = cJSON_IsString(first);
    cJSON_AddBoolToObject(meta, "first_is_string", first_is_str);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 512, 0);
    cJSON_AddStringToObject(root, "snapshot", buffer);
    (void)printed;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(arr1);
    // API sequence test completed successfully
    return 66;
}