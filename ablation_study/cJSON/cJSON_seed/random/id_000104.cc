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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *meta = cJSON_CreateObject();
    cJSON *detached_meta = (cJSON *)0;
    cJSON *new_items = (cJSON *)0;
    cJSON *got_items = (cJSON *)0;
    cJSON *cur_items = (cJSON *)0;
    int orig_size = 0;
    int new_size = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup - populate structures and attach to root
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "initial_count", 2);

    // step 3: Operate - inspect array size, detach meta, build replacement and replace
    got_items = cJSON_GetObjectItem(root, "items");
    orig_size = cJSON_GetArraySize(got_items);
    detached_meta = cJSON_DetachItemFromObject(root, "meta");
    new_items = cJSON_CreateArray();
    cJSON_AddItemToArray(new_items, cJSON_CreateString("replacement"));
    cJSON_AddItemToArray(new_items, detached_meta);
    replaced = cJSON_ReplaceItemInObject(root, "items", new_items);

    // step 4: Validate - examine new array and record sizes in root
    cur_items = cJSON_GetObjectItem(root, "items");
    new_size = cJSON_GetArraySize(cur_items);
    cJSON_AddNumberToObject(root, "orig_size", (double)orig_size);
    cJSON_AddNumberToObject(root, "new_size", (double)new_size);
    cJSON_AddNumberToObject(root, "replace_result", (double)replaced);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}