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
//<ID> 1356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *dup_str_item = (cJSON *)0;
    cJSON *dup_num_item = (cJSON *)0;
    cJSON *new_num_item = (cJSON *)0;
    const char *retrieved_text = (const char *)0;
    double retrieved_number = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create a root object with a string and a number
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.5);
    str_item = cJSON_CreateString("hello cJSON");
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddItemToObject(root, "str", str_item);

    // step 3: Configure - deep duplicate the root to exercise duplication API
    dup_root = cJSON_Duplicate(root, 1);

    // step 4: Operate - extract values from the duplicated object and reuse them
    dup_str_item = cJSON_GetObjectItem(dup_root, "str");
    dup_num_item = cJSON_GetObjectItem(dup_root, "num");
    retrieved_text = cJSON_GetStringValue(dup_str_item);
    retrieved_number = cJSON_GetNumberValue(dup_num_item);
    cJSON_AddStringToObject(root, "str_copy", retrieved_text);
    new_num_item = cJSON_CreateNumber(retrieved_number * 2.0);
    cJSON_AddItemToObject(dup_root, "double_num", new_num_item);

    // step 5: Validate - compute a small validation score based on observations
    validation_score = (int)(retrieved_text != (const char *)0) + (int)(retrieved_number == 42.5) + (int)(cJSON_GetObjectItem(root, "str_copy") != (cJSON *)0);

    // step 6: Cleanup - free allocated cJSON trees
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)retrieved_text;
    (void)retrieved_number;
    return 66;
}