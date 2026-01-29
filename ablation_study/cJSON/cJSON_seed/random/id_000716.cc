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
//<ID> 716
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *target = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *retr_greeting = (cJSON *)0;
    cJSON *retr_target = (cJSON *)0;
    cJSON *retr_flag = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_greeting = 0;
    cJSON_bool added_target = 0;
    cJSON_bool added_flag = 0;
    cJSON_bool is_greeting_str = 0;
    cJSON_bool is_target_str = 0;
    cJSON_bool is_flag_false = 0;
    int validation_sum = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello");
    target = cJSON_CreateString("world");
    flag = cJSON_CreateFalse();

    // step 3: Configure
    added_greeting = cJSON_AddItemToObject(root, "greeting", greeting);
    added_target = cJSON_AddItemToObject(root, "target", target);
    added_flag = cJSON_AddItemToObject(root, "flag", flag);

    // step 4: Operate
    retr_greeting = cJSON_GetObjectItem(root, "greeting");
    retr_target = cJSON_GetObjectItem(root, "target");
    retr_flag = cJSON_GetObjectItem(root, "flag");
    is_greeting_str = cJSON_IsString(retr_greeting);
    is_target_str = cJSON_IsString(retr_target);
    is_flag_false = cJSON_IsFalse(retr_flag);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_sum = (int)added_greeting + (int)added_target + (int)added_flag
                   + (int)is_greeting_str + (int)is_target_str + (int)is_flag_false
                   + (int)(printed != (char *)0);
    (void)validation_sum;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}