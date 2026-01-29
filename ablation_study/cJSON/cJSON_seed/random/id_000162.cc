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
//<ID> 162
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
    cJSON *answer = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    cJSON *got_answer = (cJSON *)0;
    cJSON *flag_num = (cJSON *)0;
    cJSON *double_num = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool child_is_object = (cJSON_bool)0;
    double answer_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    answer = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 4: Operate
    got_child = cJSON_GetObjectItem(root, "child");
    child_is_object = cJSON_IsObject(got_child);
    flag_num = cJSON_CreateNumber((double)child_is_object);
    cJSON_AddItemToObject(root, "child_is_object", flag_num);
    got_answer = cJSON_GetObjectItem(root, "answer");
    answer_value = cJSON_GetNumberValue(got_answer);
    double_num = cJSON_CreateNumber(answer_value * 2.0);
    cJSON_AddItemToObject(root, "answer_double", double_num);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}