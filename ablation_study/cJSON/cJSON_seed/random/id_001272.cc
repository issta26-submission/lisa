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
//<ID> 1272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *added_bool = (cJSON *)0;
    cJSON *parsed_extra = (cJSON *)0;
    cJSON_bool add_result = 0;
    int items_size = 0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON object containing an array
    root = cJSON_Parse("{\"items\":[10,20,30]}");

    // step 3: Configure - obtain the array from the parsed root
    items = cJSON_GetObjectItem(root, "items");

    // step 4: Operate - measure array size, add a boolean to root, parse an extra value and attach it
    items_size = cJSON_GetArraySize(items);
    added_bool = cJSON_AddBoolToObject(root, "enabled", 1);
    parsed_extra = cJSON_Parse("42");
    add_result = cJSON_AddItemToObject(root, "extra", parsed_extra);

    // step 5: Validate - compute a simple validation score to ensure flow through APIs
    validation_score = items_size + (int)(added_bool != (cJSON *)0) + (int)(parsed_extra != (cJSON *)0) + (int)add_result + (int)(root != (cJSON *)0) + (int)(items != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}