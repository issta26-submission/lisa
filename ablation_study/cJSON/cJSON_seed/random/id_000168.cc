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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_answer = (cJSON *)0;
    cJSON *num_sub = (cJSON *)0;
    cJSON *got_answer = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    cJSON *got_sub = (cJSON *)0;
    cJSON *flag_answer_is_obj = (cJSON *)0;
    cJSON *flag_child_is_obj = (cJSON *)0;
    cJSON_bool answer_is_obj = (cJSON_bool)0;
    cJSON_bool child_is_obj = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_answer = cJSON_CreateNumber(42.0);
    num_sub = cJSON_CreateNumber(7.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "answer", num_answer);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "sub", num_sub);

    // step 4: Operate
    got_answer = cJSON_GetObjectItem(root, "answer");
    answer_is_obj = cJSON_IsObject(got_answer);
    got_child = cJSON_GetObjectItem(root, "child");
    child_is_obj = cJSON_IsObject(got_child);
    got_sub = cJSON_GetObjectItem(got_child, "sub");

    // step 5: Validate
    flag_answer_is_obj = cJSON_CreateNumber((double)answer_is_obj);
    flag_child_is_obj = cJSON_CreateNumber((double)child_is_obj);
    cJSON_AddItemToObject(root, "answer_is_object", flag_answer_is_obj);
    cJSON_AddItemToObject(root, "child_is_object", flag_child_is_obj);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}