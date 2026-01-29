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
//<ID> 1137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *str_ref = cJSON_CreateStringReference("original_ref");
    cJSON_AddItemToArray(array, str_ref);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, true_item);
    cJSON *null_added = cJSON_AddNullToObject(root, "nullable");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(array, 0, replacement);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(array, 0);
    char *first_str = cJSON_GetStringValue(first);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double computed = (double)cJSON_IsString(first) + (double)cJSON_IsTrue(second);
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    char *buf = (char *)cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}