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
//<ID> 1038
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello-world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *copy = cJSON_CreateObject();
    cJSON *orig_answer = cJSON_GetObjectItem(root, "answer");
    double answer_val = cJSON_GetNumberValue(orig_answer);
    cJSON *answer_copy = cJSON_CreateNumber(answer_val);
    cJSON_AddItemToObject(copy, "answer", answer_copy);
    cJSON *orig_greet = cJSON_GetObjectItem(root, "greeting");
    const char *greet_val = cJSON_GetStringValue(orig_greet);
    cJSON *greet_copy = cJSON_CreateString(greet_val);
    cJSON_AddItemToObject(copy, "greeting", greet_copy);
    cJSON *flag_orig = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_true = cJSON_IsTrue(flag_orig);
    cJSON *flag_num = cJSON_CreateNumber((double)flag_true);
    cJSON_AddItemToObject(copy, "flag_numeric", flag_num);

    // step 3: Operate & Validate
    cJSON_bool equal = cJSON_Compare(root, copy, 1);
    cJSON *equal_num = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObject(root, "equal_to_copy", equal_num);
    cJSON *is_true_num = cJSON_CreateNumber((double)flag_true);
    cJSON_AddItemToObject(root, "flag_true_numeric", is_true_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}