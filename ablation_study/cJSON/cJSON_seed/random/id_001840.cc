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
//<ID> 1840
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *got_name = (cJSON *)0;
    cJSON *got_true = (cJSON *)0;
    cJSON *got_custom = (cJSON *)0;
    cJSON *added_true = (cJSON *)0;
    cJSON *added_bool = (cJSON *)0;
    const char *retrieved = (const char *)0;
    cJSON_bool add_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("example");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "name", str_item);
    added_true = cJSON_AddTrueToObject(root, "enabled");
    added_bool = cJSON_AddBoolToObject(root, "visible", 0);

    // step 4: Operate
    got_name = cJSON_GetObjectItem(root, "name");
    got_true = cJSON_GetObjectItem(root, "enabled");
    got_custom = cJSON_GetObjectItem(root, "visible");
    retrieved = cJSON_GetStringValue(got_name);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0)
               + (int)(str_item != (cJSON *)0)
               + (int)(add_ok != 0)
               + (int)(added_true != (cJSON *)0)
               + (int)(added_bool != (cJSON *)0)
               + (int)(got_name != (cJSON *)0)
               + (int)(got_true != (cJSON *)0)
               + (int)(got_custom != (cJSON *)0)
               + (int)(retrieved != (const char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)got_name;
    (void)got_true;
    (void)got_custom;
    (void)retrieved;
    (void)added_true;
    (void)added_bool;
    (void)add_ok;
    (void)str_item;
    (void)root;
    (void)validation;

    // API sequence test completed successfully
    return 66;
}