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
//<ID> 1377
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *found_greet = (cJSON *)0;
    int is_array_flag = 0;
    int validation_score = 0;
    char json_buf[] = "{\"letters\": [ \"alpha\" , \"beta\" ] , \"greet\" : \"hello\" }";

    // step 2: Initialize - create root object and an array with two string items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "letters", arr);

    // step 3: Configure - add a string member to the root using cJSON_AddStringToObject
    found_greet = cJSON_AddStringToObject(root, "greet", "hello");

    // step 4: Operate - retrieve the array and its first element, check type, and minify a JSON buffer
    found_arr = cJSON_GetObjectItem(root, "letters");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    first_item = cJSON_GetArrayItem(found_arr, 0);
    cJSON_Minify(json_buf);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(first_item != (cJSON *)0) + (int)(found_greet != (cJSON *)0) + (int)(json_buf[0] != '\0');

    // step 6: Cleanup - delete the entire root (cleans children) and finish
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)found_arr;
    (void)first_item;
    (void)found_greet;
    return 66;
}