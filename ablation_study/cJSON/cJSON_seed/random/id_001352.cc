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
//<ID> 1352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *dup_num = (cJSON *)0;
    cJSON *dup_greet = (cJSON *)0;
    const cJSON *greet_ref = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    double retrieved_num = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a numeric item
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON_AddStringToObject(root, "salutation", "hello world");

    // step 3: Configure - obtain references and duplicate items
    greet_ref = cJSON_GetObjectItem(root, "salutation");
    dup_greet = cJSON_Duplicate(greet_ref, 1);
    dup_num = cJSON_Duplicate(num, 0);

    // step 4: Operate - extract values from duplicated items
    retrieved_str = cJSON_GetStringValue(dup_greet);
    retrieved_num = cJSON_GetNumberValue(dup_num);

    // step 5: Validate - simple consistency checks aggregated into a score
    validation_score = (int)(retrieved_str != (const char *)0) + (int)(retrieved_num == 42.0) + (int)(cJSON_GetStringValue(greet_ref) != (const char *)0);

    // step 6: Cleanup - free duplicated items and the root (which frees attached children)
    cJSON_Delete(dup_greet);
    cJSON_Delete(dup_num);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)retrieved_str;
    (void)retrieved_num;
    (void)greet_ref;
    return 66;
}