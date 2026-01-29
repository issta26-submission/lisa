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
//<ID> 1844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_active = (cJSON *)0;
    cJSON *retrieved_verified = (cJSON *)0;
    const char *name_str = (const char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");

    // step 3: Configure
    (void)cJSON_AddItemToObject(root, "name", name_item);
    true_item = cJSON_AddTrueToObject(root, "active");
    bool_item = cJSON_AddBoolToObject(root, "verified", 1);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(retrieved_name);
    retrieved_active = cJSON_GetObjectItem(root, "active");
    retrieved_verified = cJSON_GetObjectItem(root, "verified");

    // step 5: Validate
    validation = (int)(retrieved_name != (cJSON *)0) + (int)(name_str != (const char *)0) + (int)(retrieved_active != (cJSON *)0) + (int)(retrieved_verified != (cJSON *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)true_item;
    (void)bool_item;
    (void)retrieved_name;
    (void)retrieved_active;
    (void)retrieved_verified;
    (void)name_item;
    (void)name_str;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}