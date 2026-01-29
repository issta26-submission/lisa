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
//<ID> 1273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *numbers = (cJSON *)0;
    cJSON *new_obj = (cJSON *)0;
    cJSON *bool_node = (cJSON *)0;
    cJSON *external = (cJSON *)0;
    int size_numbers = 0;
    int size_external = 0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON document that contains an array
    const char *json_text = "{\"numbers\": [1, 2, 3], \"info\": {\"name\":\"example\"}}";
    root = cJSON_Parse(json_text);

    // step 3: Configure - extract the numbers array and create a new object with a boolean
    numbers = cJSON_GetObjectItem(root, "numbers");
    new_obj = cJSON_CreateObject();
    bool_node = cJSON_AddBoolToObject(new_obj, "enabled", 1);
    cJSON_AddItemToObject(root, "added", new_obj);

    // step 4: Operate - parse an external array and attach it, then measure sizes
    external = cJSON_Parse("[\"x\",\"y\",\"z\"]");
    cJSON_AddItemToObject(root, "external", external);
    size_numbers = cJSON_GetArraySize(numbers);
    size_external = cJSON_GetArraySize(external);

    // step 5: Validate - compute a simple validation score to ensure data flowed through APIs
    validation_score = size_numbers + size_external + (int)(root != (cJSON *)0) + (int)(numbers != (cJSON *)0) + (int)(new_obj != (cJSON *)0) + (int)(bool_node != (cJSON *)0) + (int)(external != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}