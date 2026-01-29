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
//<ID> 1508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *child_num = (cJSON *)0;
    cJSON *child_temp = (cJSON *)0;
    cJSON *retrieved_answer = (cJSON *)0;
    cJSON *retrieved_temp = (cJSON *)0;
    char *printed = (char *)0;
    const char *greeting_value = (const char *)0;
    double answer_value = 0.0;
    int validation_score = 0;
    cJSON_bool added_greeting = 0;
    cJSON_bool added_answer = 0;
    cJSON_bool added_temp = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_str = cJSON_CreateString("hello world");
    child_num = cJSON_CreateNumber(123.0);
    child_temp = cJSON_CreateString("transient");

    // step 3: Configure
    added_greeting = cJSON_AddItemToObjectCS(root, "greeting", child_str);
    added_answer = cJSON_AddItemToObjectCS(root, "answer", child_num);
    added_temp = cJSON_AddItemToObjectCS(root, "temp", child_temp);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_answer = cJSON_GetObjectItem(root, "answer");
    answer_value = cJSON_GetNumberValue(retrieved_answer);
    greeting_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));

    // step 5: Validate
    cJSON_DeleteItemFromObject(root, "temp");
    retrieved_temp = cJSON_GetObjectItem(root, "temp");
    validation_score = (int)(printed != (char *)0) + (int)added_greeting + (int)added_answer + (int)(answer_value == 123.0) + (int)(greeting_value != (const char *)0) + (int)(retrieved_temp == (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)greeting_value;
    (void)answer_value;
    (void)retrieved_answer;
    (void)retrieved_temp;
    // API sequence test completed successfully
    return 66;
}