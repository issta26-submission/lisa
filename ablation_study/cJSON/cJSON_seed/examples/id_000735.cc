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
//<ID> 735
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
    cJSON *sref1 = cJSON_CreateStringReference("alpha");
    cJSON *sref2 = cJSON_CreateStringReference("beta");
    cJSON_AddItemToArray(arr, sref1);
    cJSON_AddItemToArray(arr, sref2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *label = cJSON_CreateString("root_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Operate (serialize and prepare buffer)
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);

    // step 3: Parse & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *first = cJSON_GetArrayItem(parsed_items, 0);
    const char *first_val = cJSON_GetStringValue(first);
    (void)first_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}