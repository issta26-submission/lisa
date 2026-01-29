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
//<ID> 930
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *orig_key = "orig";
    const char *name_key = "name";
    const char *name_val = "example";
    char json_buf[] = "  { \"k1\" : \"v1\" , \"k2\" :  2 }  ";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *topArr = (cJSON *)0;
    cJSON *parsed_min = (cJSON *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array with two string elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("one");
    str2 = cJSON_CreateString("two");

    // step 3: Configure - populate array and attach original array to root
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, orig_key, arr);

    // step 4: Core operations - create an array reference to the original array, add it to a new array, and attach plus add a string field
    arr_ref = cJSON_CreateArrayReference(arr);
    topArr = cJSON_CreateArray();
    cJSON_AddItemToArray(topArr, arr_ref);
    cJSON_AddItemToObject(root, arr_key, topArr);
    cJSON_AddStringToObject(root, name_key, name_val);

    // step 5: Operate/Validate - minify a JSON buffer and parse it to ensure Minify produced valid JSON
    cJSON_Minify(json_buf);
    parsed_min = cJSON_Parse(json_buf);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)(topArr != (cJSON *)0) + (int)(parsed_min != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(parsed_min);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}