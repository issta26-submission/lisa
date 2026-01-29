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
//<ID> 884
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool added_str = (cJSON_bool)0;
    char *printed = (char *)0;
    const cJSON *retrieved_str = (const cJSON *)0;
    const cJSON *retrieved_num = (const cJSON *)0;
    const char *greeting = (const char *)0;
    double answer_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize root object and create a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello world");

    // step 3: Configure object by adding items using required APIs
    added_str = cJSON_AddItemToObject(root, "greeting", str_item);
    true_item = cJSON_AddTrueToObject(root, "enabled");
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 4: Operate / Validate by retrieving values and printing
    retrieved_str = cJSON_GetObjectItem(root, "greeting");
    greeting = cJSON_GetStringValue(retrieved_str);
    retrieved_num = cJSON_GetObjectItem(root, "answer");
    answer_value = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)added_str + (printed != (char *)0) + (greeting != (const char *)0) + (int)(answer_value == 42.0);

    // step 5: Cleanup
    (void)validation_score;
    cJSON_free(printed);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}