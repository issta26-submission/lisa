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
//<ID> 1270
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
    cJSON *new_num = (cJSON *)0;
    cJSON *sub = (cJSON *)0;
    cJSON *flag_node = (cJSON *)0;
    cJSON_bool add_result = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;
    const char json_text[] = "{\"data\":[1,2,3]}";

    // step 2: Initialize - parse JSON into a root object
    root = cJSON_Parse(json_text);

    // step 3: Configure - locate array, measure size, create and append a number, create a sub-object and add a boolean to it
    arr = cJSON_GetObjectItem(root, "data");
    size_before = cJSON_GetArraySize(arr);
    new_num = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(arr, new_num);
    sub = cJSON_CreateObject();
    add_result = cJSON_AddItemToObject(root, "sub", sub);
    flag_node = cJSON_AddBoolToObject(sub, "ok", 1);

    // step 4: Operate - measure array size after modification
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation score to ensure flow through APIs
    validation_score = size_before + size_after + (int)(root != (cJSON *)0) + (int)(new_num != (cJSON *)0) + (int)(flag_node != (cJSON *)0) + (int)add_result;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}