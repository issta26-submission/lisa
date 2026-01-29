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
//<ID> 1841
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
    cJSON *retrieved_str = (cJSON *)0;
    cJSON *retrieved_true = (cJSON *)0;
    cJSON *retrieved_bool = (cJSON *)0;
    char *strval = (char *)0;
    cJSON_bool add_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("example-name");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "name", str_item);
    true_item = cJSON_AddTrueToObject(root, "enabled");
    bool_item = cJSON_AddBoolToObject(root, "visible", (cJSON_bool)0);

    // step 4: Operate
    retrieved_str = cJSON_GetObjectItem(root, "name");
    retrieved_true = cJSON_GetObjectItem(root, "enabled");
    retrieved_bool = cJSON_GetObjectItem(root, "visible");
    strval = cJSON_GetStringValue(retrieved_str);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(retrieved_str != (cJSON *)0) + (int)(retrieved_true != (cJSON *)0) + (int)(retrieved_bool != (cJSON *)0) + (int)(strval != (char *)0) + (int)(add_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)str_item;
    (void)true_item;
    (void)bool_item;
    (void)retrieved_str;
    (void)retrieved_true;
    (void)retrieved_bool;
    (void)strval;
    (void)add_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}