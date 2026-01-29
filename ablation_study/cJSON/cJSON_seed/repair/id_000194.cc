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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON *elem1 = cJSON_CreateString("alpha");
    cJSON *elem2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, elem1);
    cJSON_AddItemToArray(items, elem2);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *status = cJSON_GetObjectItem(root, "status");
    const char *status_str = cJSON_GetStringValue(status);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_num);
    cJSON *status_copy = cJSON_CreateString(status_str);
    cJSON_AddItemToObject(root, "status_copy", status_copy);
    (void)first_str; // used logically in flow by deriving status_copy from status and counting items

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}