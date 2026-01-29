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
//<ID> 932
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *str_key = "title";
    const char *str_val = "example";
    char json_buf[] = " { \"a\" : 1 , \"b\" : 2 } ";
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *array_ref = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool added_to_array = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup
    cJSON_Minify(json_buf);
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    child = cJSON_CreateNumber(123.0);

    // step 3: Configure
    array_ref = cJSON_CreateArrayReference(child);
    added_to_array = cJSON_AddItemToArray(array, array_ref);
    cJSON_AddItemToObject(root, arr_key, array);

    // step 4: Operate
    str_item = cJSON_AddStringToObject(root, str_key, str_val);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(array != (cJSON *)0) + (int)(child != (cJSON *)0) + (int)(array_ref != (cJSON *)0) + (int)added_to_array + (int)(str_item != (cJSON *)0) + (int)(json_buf[0] != '\0');

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}