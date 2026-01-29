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
//<ID> 1374
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
    cJSON *child = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *found_child = (cJSON *)0;
    const char *inner_str = (const char *)0;
    int is_array_flag = 0;
    int validation_score = 0;
    char minify_src[] = "  { \"x\" : [ \"a\" , \"b\" ] , \"y\" : { \"z\" : \"v\" } }  ";

    // step 2: Initialize - create root, array and string items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("a");
    s2 = cJSON_CreateString("b");

    // step 3: Configure - attach array and child object, and add a direct string to root
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "list", arr);
    child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "inner", "hello");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 4: Operate - retrieve members, inspect types and array elements, and minify a JSON buffer
    found_arr = cJSON_GetObjectItem(root, "list");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    elem0 = cJSON_GetArrayItem(found_arr, 0);
    elem1 = cJSON_GetArrayItem(found_arr, 1);
    found_child = cJSON_GetObjectItem(root, "child");
    inner_str = cJSON_GetStringValue(cJSON_GetObjectItem(found_child, "inner"));
    cJSON_Minify(minify_src);

    // step 5: Validate - build a simple validation score
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(elem0 != (cJSON *)0) + (int)(elem1 != (cJSON *)0) + (int)(inner_str != (const char *)0);

    // step 6: Cleanup - delete created structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)minify_src;
    (void)elem0;
    (void)elem1;
    (void)found_arr;
    (void)found_child;
    (void)inner_str;
    return 66;
}