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
//<ID> 1501
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_greeting = (cJSON *)0;
    cJSON *child_answer = (cJSON *)0;
    cJSON *child_nested = (cJSON *)0;
    cJSON *nested_inner = (cJSON *)0;
    cJSON *retrieved_answer = (cJSON *)0;
    cJSON *retrieved_greeting = (cJSON *)0;
    char *printed = (char *)0;
    const char *greet_text = (const char *)0;
    double answer_value = 0.0;
    cJSON_bool added_greet = 0;
    cJSON_bool added_answer = 0;
    cJSON_bool added_nested = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_greeting = cJSON_CreateString("hello_world");
    child_answer = cJSON_CreateNumber(42.0);
    child_nested = cJSON_CreateObject();
    nested_inner = cJSON_CreateString("inner_value");

    // step 3: Configure
    added_greet = cJSON_AddItemToObjectCS(root, "greeting", child_greeting);
    added_answer = cJSON_AddItemToObjectCS(root, "answer", child_answer);
    added_nested = cJSON_AddItemToObjectCS(child_nested, "inner", nested_inner);
    cJSON_AddItemToObjectCS(root, "nested", child_nested);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_answer = cJSON_GetObjectItem(root, "answer");
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    answer_value = cJSON_GetNumberValue(retrieved_answer);
    greet_text = cJSON_GetStringValue(retrieved_greeting);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0)
                     + (int)added_greet
                     + (int)added_answer
                     + (int)added_nested
                     + (int)(retrieved_answer != (cJSON *)0)
                     + (int)(retrieved_greeting != (cJSON *)0)
                     + (int)(answer_value == 42.0)
                     + (int)(greet_text != (const char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_DeleteItemFromObject(root, "answer");
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_answer;
    (void)retrieved_greeting;
    (void)greet_text;
    // API sequence test completed successfully
    return 66;
}