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
//<ID> 1373
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
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *arr_item = (cJSON *)0;
    char *unformatted = (char *)0;
    const char *item_str = (const char *)0;
    int is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with two strings
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("alpha");
    s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "letters", arr);

    // step 3: Configure - add an independent status string to the root using AddStringToObject
    cJSON_AddStringToObject(root, "status", "ok");

    // step 4: Operate - retrieve the array, verify it's an array, extract an element, produce unformatted JSON and minify it
    found_arr = cJSON_GetObjectItem(root, "letters");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    arr_item = cJSON_GetArrayItem(found_arr, 1);
    item_str = cJSON_GetStringValue(arr_item);
    unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(unformatted);

    // step 5: Validate - compose a simple validation score from the results
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(arr_item != (cJSON *)0) + (int)(item_str != (const char *)0) + (int)(unformatted != (char *)0);

    // step 6: Cleanup - free the printed string and delete the JSON tree
    if (unformatted) { cJSON_free(unformatted); } /* single straight-line cleanup call allowed */
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)item_str;
    (void)arr_item;
    return 66;
}