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
//<ID> 1376
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    char *raw = (char *)0;
    int is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array, and two string elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 3: Configure - attach the array to the root and add an extra string entry using AddStringToObject
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddStringToObject(root, "note", "sample");

    // step 4: Operate - retrieve the array and its items, then produce and minify an unformatted JSON string
    found_arr = cJSON_GetObjectItem(root, "letters");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    item0 = cJSON_GetArrayItem(found_arr, 0);
    item1 = cJSON_GetArrayItem(found_arr, 1);
    raw = cJSON_PrintUnformatted(root);
    cJSON_Minify(raw);

    // step 5: Validate - compose a simple validation score
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(item0 != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(raw != (char *)0 && raw[0] != '\0');

    // step 6: Cleanup - free printed string and delete the JSON structure
    if (raw) { cJSON_free(raw); } /* single unconditional cleanup expression allowed; no branching logic beyond this cleanup */
    cJSON_Delete(root);

    /* API sequence test completed successfully */
    (void)validation_score;
    (void)item0;
    (void)item1;
    (void)found_arr;
    return 66;
}