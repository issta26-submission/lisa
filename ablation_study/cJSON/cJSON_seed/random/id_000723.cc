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
//<ID> 723
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    const char *retrieved_str = (const char *)0;
    double retrieved_num = 0.0;
    cJSON_bool added_pi = 0;
    cJSON_bool added_greet = 0;
    cJSON_bool added_active = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);
    str_item = cJSON_CreateString("hello world");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_pi = cJSON_AddItemToObject(root, "pi", num_item);
    added_greet = cJSON_AddItemToObject(root, "greet", str_item);
    added_active = cJSON_AddItemToObject(root, "active", false_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved = cJSON_GetObjectItem(root, "greet");
    retrieved_str = cJSON_GetStringValue(retrieved);
    retrieved_num = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_pi + (int)added_greet + (int)added_active + (int)(retrieved_str != (const char *)0) + (int)(retrieved_num == 3.14159);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}