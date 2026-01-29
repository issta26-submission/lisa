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
//<ID> 1408
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int initial_nums[] = {1, 2, 3};
    cJSON *nums = cJSON_CreateIntArray(initial_nums, 3);
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON *answer = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "answer", answer);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 2: Configure
    int replacement_nums[] = {10, 20};
    cJSON *new_nums = cJSON_CreateIntArray(replacement_nums, 2);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "nums", new_nums);
    (void)replaced;

    // step 3: Operate & Validate
    cJSON *got_answer = cJSON_GetObjectItem(root, "answer");
    cJSON *got_greeting = cJSON_GetObjectItem(root, "greeting");
    cJSON *got_nums = cJSON_GetObjectItem(root, "nums");
    double answer_value = cJSON_GetNumberValue(got_answer);
    char *greeting_value = cJSON_GetStringValue(got_greeting);
    int nums_count = cJSON_GetArraySize(got_nums);
    cJSON *first_num = cJSON_GetArrayItem(got_nums, 0);
    double first_num_value = cJSON_GetNumberValue(first_num);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    (void)answer_value;
    (void)greeting_value;
    (void)nums_count;
    (void)first_num_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}