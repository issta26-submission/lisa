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
//<ID> 1271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *bool_node = (cJSON *)0;
    int arr_size = 0;
    int validation_score = 0;
    const char *json_text = "{\"items\":[10,20,30], \"name\":\"example\"}";

    // step 2: Initialize - parse JSON into a root object
    root = cJSON_Parse(json_text);

    // step 3: Configure - obtain the array from the parsed object and prepare a meta object with a boolean
    arr = cJSON_GetObjectItem(root, "items");
    meta = cJSON_CreateObject();
    bool_node = cJSON_AddBoolToObject(meta, "valid", 1);

    // step 4: Operate - attach the meta object to root and measure the array size
    cJSON_AddItemToObject(root, "meta", meta);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation score to exercise values
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(meta != (cJSON *)0) + (int)(bool_node != (cJSON *)0) + arr_size;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}