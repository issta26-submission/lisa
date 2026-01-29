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
//<ID> 1843
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
    cJSON *true_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *got_str = (cJSON *)0;
    cJSON *got_true = (cJSON *)0;
    cJSON *got_bool = (cJSON *)0;
    const char *str_value = (const char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("test-string");

    // step 3: Configure
    (void)cJSON_AddItemToObject(root, "name", str_item);
    true_item = cJSON_AddTrueToObject(root, "is_active");
    bool_item = cJSON_AddBoolToObject(root, "is_visible", (cJSON_bool)0);

    // step 4: Operate
    got_str = cJSON_GetObjectItem(root, "name");
    got_true = cJSON_GetObjectItem(root, "is_active");
    got_bool = cJSON_GetObjectItem(root, "is_visible");
    str_value = cJSON_GetStringValue(got_str);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(got_str != (cJSON *)0) + (int)(got_true != (cJSON *)0) + (int)(got_bool != (cJSON *)0) + (int)(str_value != (const char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)got_str;
    (void)got_true;
    (void)got_bool;
    (void)str_value;
    (void)str_item;
    (void)true_item;
    (void)bool_item;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}