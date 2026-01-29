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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *active_item = (cJSON *)0;
    cJSON *got_meta = (cJSON *)0;
    cJSON *got_name = (cJSON *)0;
    const char *name_val = (const char *)0;
    cJSON *name_copy = (cJSON *)0;
    cJSON_bool active_state = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");

    // step 3: Configure
    cJSON_AddItemToObject(meta, "name", name_item);
    active_item = cJSON_AddTrueToObject(meta, "active");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 4: Operate / Validate
    got_meta = cJSON_GetObjectItem(root, "meta");
    got_name = cJSON_GetObjectItem(got_meta, "name");
    name_val = cJSON_GetStringValue(got_name);
    name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    active_state = cJSON_IsTrue(cJSON_GetObjectItem(got_meta, "active"));

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}