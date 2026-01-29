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
//<ID> 936
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *name_key = "name";
    const char *name_val = "example_name";
    char json_buf[] = " { \"alpha\" : 1 , \"beta\" : \"text\" } ";
    cJSON *rootObj = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON_bool added_to_array = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create object and array containers
    rootObj = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - add a string to the object and create an array reference to that string
    str_item = cJSON_AddStringToObject(rootObj, name_key, name_val);
    arr_ref = cJSON_CreateArrayReference((const cJSON *)str_item);
    added_to_array = cJSON_AddItemToArray(arr, arr_ref);

    // step 4: Operate - minify a sample JSON buffer to exercise Minify
    cJSON_Minify(json_buf);

    // step 5: Validate - derive a simple validation score from created objects and operations
    validation_score = (int)(rootObj != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)added_to_array + (int)(json_buf[0] != '\0');
    (void)validation_score;

    // step 6: Cleanup - delete created cJSON structures
    cJSON_Delete(arr);
    cJSON_Delete(rootObj);

    // API sequence test completed successfully
    return 66;
}