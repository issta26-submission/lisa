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
//<ID> 1073
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(7.0);
    cJSON *s_entry = cJSON_CreateString("entry");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s_entry);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddStringToObject(root, "status", "ready");
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate & Validate
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    const char *status_text = cJSON_GetStringValue(status_item);
    cJSON *list_item = cJSON_GetObjectItem(root, "list");
    int list_count = cJSON_GetArraySize(list_item);
    cJSON_AddNumberToObject(root, "count", (double)list_count);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)status_text;
    return 66; // API sequence test completed successfully
}