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
//<ID> 1802
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *item_a = (cJSON *)0;
    cJSON *item_b = (cJSON *)0;
    cJSON *replacement_false = (cJSON *)0;
    cJSON *dup_a = (cJSON *)0;
    cJSON *retrieved_a = (cJSON *)0;
    const char *dup_str = (const char *)0;
    cJSON_bool replace_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item_a = cJSON_CreateString("one");
    item_b = cJSON_CreateString("two");
    cJSON_AddItemToObject(root, "a", item_a);
    cJSON_AddItemToObject(root, "b", item_b);

    // step 3: Configure
    dup_a = cJSON_Duplicate(item_a, 1);
    replacement_false = cJSON_CreateFalse();

    // step 4: Operate
    retrieved_a = cJSON_GetObjectItem(root, "a");
    replace_ok = cJSON_ReplaceItemViaPointer(root, retrieved_a, replacement_false);
    dup_str = cJSON_GetStringValue(dup_a);
    cJSON_AddItemToObject(root, "a_copy", dup_a);

    // step 5: Validate
    validation = (int)(retrieved_a != (cJSON *)0) + (int)(replace_ok != 0) + (int)(dup_str != (const char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)item_a;
    (void)item_b;
    (void)replacement_false;
    (void)dup_a;
    (void)retrieved_a;
    (void)dup_str;
    (void)replace_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}