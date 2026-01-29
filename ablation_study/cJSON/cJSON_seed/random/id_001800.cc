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
//<ID> 1800
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *s_old = (cJSON *)0;
    cJSON *s_new = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *retrieved_before = (cJSON *)0;
    cJSON *retrieved_after = (cJSON *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool replace_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s_old = cJSON_CreateString("value_before");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "item", s_old);
    add_ok2 = cJSON_AddItemToObject(root, "enabled", flag_false);
    retrieved_before = cJSON_GetObjectItem(root, "item");

    // step 4: Operate
    s_new = cJSON_CreateString("value_after");
    replace_ok = cJSON_ReplaceItemViaPointer(root, retrieved_before, s_new);
    retrieved_after = cJSON_GetObjectItem(root, "item");

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(retrieved_before != (cJSON *)0) + (int)(retrieved_after != (cJSON *)0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(replace_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)s_old;
    (void)s_new;
    (void)flag_false;
    (void)retrieved_before;
    (void)retrieved_after;
    (void)add_ok1;
    (void)add_ok2;
    (void)replace_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}