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
//<ID> 542
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
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, t);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure and operate
    cJSON *detached_items = cJSON_DetachItemFromObject(root, "items");
    cJSON *picked = cJSON_GetArrayItem(detached_items, 1);
    char *picked_val = cJSON_GetStringValue(picked);
    cJSON *summary = cJSON_CreateObject();
    cJSON *copied_name = cJSON_CreateString(picked_val);
    cJSON_AddItemToObject(summary, "first_string", copied_name);
    cJSON_AddItemToObject(root, "summary", summary);
    cJSON *num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(detached_items, num);
    cJSON_AddItemToObject(root, "items_attached", detached_items);

    // step 3: Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *got_summary = cJSON_GetObjectItem(root, "summary");
    cJSON *got_first = cJSON_GetObjectItem(got_summary, "first_string");
    char *validated = cJSON_GetStringValue(got_first);
    (void)validated; // flow of data validated by use in creation above

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}