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
//<ID> 720
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
    cJSON *retrieved = (cJSON *)0;
    char *retrieved_str = (char *)0;
    char *printed = (char *)0;
    cJSON_bool added_str = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello world");
    num_item = cJSON_CreateNumber(42.0);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_str = cJSON_AddItemToObject(root, "greeting", str_item);
    added_num = cJSON_AddItemToObject(root, "answer", num_item);
    added_false = cJSON_AddItemToObject(root, "active", false_item);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "greeting");
    retrieved_str = cJSON_GetStringValue(retrieved);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_str + (int)added_num + (int)added_false + (retrieved_str != (char *)0) + (printed != (char *)0) + (int)cJSON_IsFalse(cJSON_GetObjectItem(root, "active"));
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}