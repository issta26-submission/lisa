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
//<ID> 2463
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
    cJSON *true_item = cJSON_AddTrueToObject(child, "active");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "config", child);
    cJSON_AddItemToObject(root, "disabled", false_item);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON_bool is_false = cJSON_IsFalse(false_item);
    buffer[1] = (char)(int)is_false;
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON *answer_ref = cJSON_GetObjectItem(root, "answer");
    double answer_val = cJSON_GetNumberValue(answer_ref);
    buffer[2] = (char)(int)answer_val;
    cJSON_bool true_is_false = cJSON_IsFalse(true_item);
    buffer[3] = (char)(int)true_is_false;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}