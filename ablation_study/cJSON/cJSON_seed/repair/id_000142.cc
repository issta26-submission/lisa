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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(root, "title", "cJSON Test");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s2);
    cJSON_AddStringToObject(meta, "env", "production");

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(meta, "first_item", first_val);
    cJSON *title = cJSON_GetObjectItem(root, "title");
    const char *title_val = cJSON_GetStringValue(title);
    cJSON_AddStringToObject(root, "title_copy", title_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    (void)size;
    (void)title_val;
    return 66;
    // API sequence test completed successfully
}