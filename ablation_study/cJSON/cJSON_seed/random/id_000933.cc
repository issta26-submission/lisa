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
//<ID> 933
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *name_key = "label";
    const char *name_val = "test_label";
    char json_raw[] = " { \"a\" : 1 , \"b\" : 2 } ";
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, and a standalone child string
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    child_str = cJSON_CreateString("child_value");

    // step 3: Configure - create an array reference to the standalone child and add it to the array, then attach array to root
    arr_ref = cJSON_CreateArrayReference((const cJSON *)child_str);
    cJSON_AddItemToArray(array, arr_ref);
    cJSON_AddItemToObject(root, arr_key, array);

    // step 4: Operate - add a string to the root using AddStringToObject and minify an external JSON buffer
    str_item = cJSON_AddStringToObject(root, name_key, name_val);
    cJSON_Minify(json_raw);

    // step 5: Validate - derive a simple validation score from created pointers and the minified buffer content
    validation_score = (int)(root != (cJSON *)0) + (int)(array != (cJSON *)0) + (int)(child_str != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(json_raw[0] != '\0');
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(child_str);

    // API sequence test completed successfully
    return 66;
}