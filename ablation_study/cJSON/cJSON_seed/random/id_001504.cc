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
//<ID> 1504
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_name = (cJSON *)0;
    cJSON *child_age = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    char *printed_before = (char *)0;
    char *printed_after = (char *)0;
    const char *name_val = (const char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_age = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_name = cJSON_CreateString("Alice");
    child_age = cJSON_CreateNumber(30.0);

    // step 3: Configure
    added_name = cJSON_AddItemToObjectCS(root, "name", child_name);
    added_age = cJSON_AddItemToObjectCS(root, "age", child_age);

    // step 4: Operate
    printed_before = cJSON_PrintUnformatted(root);
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_val = cJSON_GetStringValue(retrieved_name);
    cJSON_DeleteItemFromObject(root, "age");
    printed_after = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)(printed_before != (char *)0) + (int)(printed_after != (char *)0) + (int)added_name + (int)added_age + (int)(retrieved_name != (cJSON *)0) + (int)(name_val != (const char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed_before);
    cJSON_free((void *)printed_after);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_name;
    (void)name_val;
    (void)added_age;
    // API sequence test completed successfully
    return 66;
}