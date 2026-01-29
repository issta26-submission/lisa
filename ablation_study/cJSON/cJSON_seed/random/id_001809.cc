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
//<ID> 1809
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *parent = (cJSON *)0;
    cJSON *s_old = (cJSON *)0;
    cJSON *s_other = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *item_ptr = (cJSON *)0;
    cJSON *retrieved_parent = (cJSON *)0;
    cJSON *replaced_item = (cJSON *)0;
    const char *strval = (const char *)0;
    cJSON_bool replace_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    parent = cJSON_CreateObject();
    s_old = cJSON_CreateString("alpha");
    s_other = cJSON_CreateString("beta");
    flag_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(parent, "target", s_old);
    cJSON_AddItemToObject(parent, "other", s_other);
    cJSON_AddItemToObject(parent, "flag", flag_item);
    cJSON_AddItemToObject(root, "parent", parent);

    // step 4: Operate
    item_ptr = cJSON_GetObjectItem(parent, "target");
    replacement = cJSON_CreateString("replaced");
    replace_ok = cJSON_ReplaceItemViaPointer(parent, item_ptr, replacement);
    retrieved_parent = cJSON_GetObjectItem(root, "parent");
    replaced_item = cJSON_GetObjectItem(retrieved_parent, "target");
    strval = cJSON_GetStringValue(replaced_item);

    // step 5: Validate
    validation = (int)(replace_ok != 0) + (int)(strval != (const char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)parent;
    (void)s_old;
    (void)s_other;
    (void)flag_item;
    (void)replacement;
    (void)item_ptr;
    (void)retrieved_parent;
    (void)replaced_item;
    (void)strval;
    (void)replace_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}