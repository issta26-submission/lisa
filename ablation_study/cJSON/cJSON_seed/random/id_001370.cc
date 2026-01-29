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
//<ID> 1370
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *parsed_root = (cJSON *)0;
    const char *s0 = (const char *)0;
    const char *s1 = (const char *)0;
    char json_buf[] = "{ \"parsed\": { \"greet\" : \"hello\" }, \"list\" : [ \"one\" , \"two\" ] }";
    int is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with two string elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));

    // step 3: Configure - attach the array and two named strings to the root object
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddStringToObject(root, "alpha", "A");
    cJSON_AddStringToObject(root, "beta", "B");

    // step 4: Operate - minify a JSON buffer then parse it; inspect array members from original root
    cJSON_Minify(json_buf);
    parsed_root = cJSON_Parse(json_buf);
    found_arr = cJSON_GetObjectItem(root, "letters");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    elem0 = cJSON_GetArrayItem(found_arr, 0);
    elem1 = cJSON_GetArrayItem(found_arr, 1);
    s0 = cJSON_GetStringValue(elem0);
    s1 = cJSON_GetStringValue(elem1);

    // step 5: Validate - simple aggregated checks
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(s0 != (const char *)0) + (int)(s1 != (const char *)0) + (int)(parsed_root != (cJSON *)0);

    // step 6: Cleanup - free parsed and constructed cJSON trees
    cJSON_Delete(parsed_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)s0;
    (void)s1;
    return 66;
}