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
//<ID> 1805
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *container = (cJSON *)0;
    cJSON *s_old = (cJSON *)0;
    cJSON *s_new = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *retrieved_container = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    cJSON *new_item = (cJSON *)0;
    const char *strval = (const char *)0;
    cJSON_bool replace_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    container = cJSON_CreateObject();
    s_old = cJSON_CreateString("old_value");
    s_new = cJSON_CreateString("new_value");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(container, "key", s_old);
    cJSON_AddItemToObject(root, "container", container);
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 4: Operate
    retrieved_container = cJSON_GetObjectItem(root, "container");
    retrieved_item = cJSON_GetObjectItem(retrieved_container, "key");
    replace_ok = cJSON_ReplaceItemViaPointer(retrieved_container, retrieved_item, s_new);
    new_item = cJSON_GetObjectItem(retrieved_container, "key");
    strval = cJSON_GetStringValue(new_item);
    (void)replace_ok;

    // step 5: Validate
    validation = (int)(retrieved_container != (cJSON *)0) + (int)(retrieved_item != (cJSON *)0) + (int)(new_item != (cJSON *)0) + (int)(strval != (const char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)container;
    (void)s_old;
    (void)s_new;
    (void)flag_false;
    (void)retrieved_container;
    (void)retrieved_item;
    (void)new_item;
    (void)strval;
    (void)replace_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}