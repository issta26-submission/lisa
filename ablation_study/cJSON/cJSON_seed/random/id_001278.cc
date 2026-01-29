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
//<ID> 1278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "{\"items\":[10,20,30], \"meta\":{}}";
    const char *attached_json = "{\"note\":\"attached\",\"value\":7}";
    cJSON *root = (cJSON *)0;
    cJSON *items_arr = (cJSON *)0;
    cJSON *attached = (cJSON *)0;
    cJSON *bool_node = (cJSON *)0;
    cJSON_bool enabled_flag = (cJSON_bool)1;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize - parse the main JSON and a small attached object
    root = cJSON_Parse(json);
    attached = cJSON_Parse(attached_json);

    // step 3: Configure - locate the array and measure its size, add a boolean to the parsed root
    items_arr = cJSON_GetObjectItem(root, "items");
    size_before = cJSON_GetArraySize(items_arr);
    bool_node = cJSON_AddBoolToObject(root, "enabled", enabled_flag);

    // step 4: Operate - attach the parsed object into the root and re-measure sizes for validation
    cJSON_AddItemToObject(root, "attached", attached);
    size_after = cJSON_GetArraySize(items_arr);

    // step 5: Validate - form a simple validation metric to ensure data flowed through the APIs
    validation_score = size_before + size_after + (int)(root != (cJSON *)0) + (int)(items_arr != (cJSON *)0) + (int)(attached != (cJSON *)0) + (int)(bool_node != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items and boolean node
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}