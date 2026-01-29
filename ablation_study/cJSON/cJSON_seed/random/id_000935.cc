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
//<ID> 935
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *name_key = "name";
    const char *raw_key = "raw";
    const char *name_val = "example";
    char json_buf[] = " { \"a\" : 1 , \"b\" : [ 1, 2 ] } ";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool added_array = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, a child element and an array reference to it
    root = cJSON_CreateObject();
    child = cJSON_CreateString("element");
    arr_ref = cJSON_CreateArrayReference(child);
    arr = cJSON_CreateArray();

    // step 3: Core operations - populate the array and attach the reference into it, then attach array to root
    num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, arr_ref);
    added_array = cJSON_AddItemToObject(root, arr_key, arr);

    // step 4: Operate - add a named string to root and minify a JSON buffer, then add it as another string
    str_item = cJSON_AddStringToObject(root, name_key, name_val);
    cJSON_Minify(json_buf);
    cJSON_AddStringToObject(root, raw_key, json_buf);

    // step 5: Validate - compute a simple validation score from object state
    validation_score = (int)(root != (cJSON *)0) + (int)added_array + (int)(str_item != (cJSON *)0) + (int)(cJSON_GetArraySize(arr) >= 2);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}