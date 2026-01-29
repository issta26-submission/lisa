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
//<ID> 314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    cJSON *got_name = (cJSON *)0;
    const char *retrieved = (const char *)0;
    cJSON_bool add_result = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    name_item = cJSON_CreateString("child_name_value");

    // step 3: Configure
    add_result = cJSON_AddItemToObject(child, "name", name_item);
    cJSON_AddTrueToObject(root, "status");
    add_result = cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    got_child = cJSON_GetObjectItem(root, "child");
    got_name = cJSON_GetObjectItem(got_child, "name");
    retrieved = cJSON_GetStringValue(got_name);

    // step 5: Validate
    cJSON_AddItemToObject(root, "retrieved_name", cJSON_CreateString(retrieved));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}