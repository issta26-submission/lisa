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
//<ID> 1599
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "answer", num_item);
    cJSON_AddNumberToObject(root, "direct_pi", 3.1415);
    cJSON *str_item = cJSON_CreateString("hello world");
    cJSON_AddItemReferenceToObject(root, "greeting_ref", str_item);

    // step 3: Operate and Validate
    cJSON *got_answer = cJSON_GetObjectItemCaseSensitive(child, "answer");
    double answer_val = cJSON_GetNumberValue(got_answer);
    cJSON *double_item = cJSON_CreateNumber(answer_val * 2.0);
    cJSON_AddItemToObject(root, "double_answer", double_item);
    cJSON *got_greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting_ref");
    char *greeting = cJSON_GetStringValue(got_greeting);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = greeting[0];
    buffer[1] = (char)('0' + ((int)answer_val % 10));

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}