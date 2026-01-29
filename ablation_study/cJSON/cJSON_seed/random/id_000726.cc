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
//<ID> 726
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
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_str_item = (cJSON *)0;
    char *str_value = (char *)0;
    char *printed = (char *)0;
    double num_value = 0.0;
    cJSON_bool added_s = 0;
    cJSON_bool added_n = 0;
    cJSON_bool added_f = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello world");
    num_item = cJSON_CreateNumber(3.1415);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_s = cJSON_AddItemToObject(root, "greeting", str_item);
    added_n = cJSON_AddItemToObject(root, "pi", num_item);
    added_f = cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate
    retrieved_str_item = cJSON_GetObjectItem(root, "greeting");
    str_value = cJSON_GetStringValue(retrieved_str_item);
    printed = cJSON_PrintUnformatted(root);
    num_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));

    // step 5: Validate
    validation_score = (int)(str_value != (char *)0) + (int)added_s + (int)added_n + (int)added_f + (printed != (char *)0) + (int)cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}