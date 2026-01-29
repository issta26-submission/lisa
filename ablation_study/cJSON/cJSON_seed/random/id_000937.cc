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
//<ID> 937
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *items_key = "items";
    const char *raw_key = "raw";
    const char *child_val = "child_value";
    char json_buf[] = "  { \"note\" : \"note val\" }  ";
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *extra_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool added_to_array = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a child string
    root = cJSON_CreateObject();
    child = cJSON_CreateString(child_val);

    // step 3: Configure - create an array reference to the child and add another item into that array
    arr_ref = cJSON_CreateArrayReference(child);
    extra_item = cJSON_CreateString("extra_element");
    added_to_array = cJSON_AddItemToArray(arr_ref, extra_item);
    cJSON_AddItemToObject(root, items_key, arr_ref);

    // step 4: Operate - minify a JSON buffer and add it as a string to the root object
    cJSON_Minify(json_buf);
    str_item = cJSON_AddStringToObject(root, raw_key, json_buf);

    // step 5: Validate - aggregate simple validation indicators
    validation_score = (int)(root != (cJSON *)0) + (int)(child != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)added_to_array + (int)(str_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}