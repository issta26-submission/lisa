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
//<ID> 1500
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *child_num = (cJSON *)0;
    cJSON *retrieved_before = (cJSON *)0;
    cJSON *retrieved_after = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_cs = 0;
    cJSON_bool added_norm = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_str = cJSON_CreateString("example");
    child_num = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_cs = cJSON_AddItemToObjectCS(root, "key_cs", child_str);
    added_norm = cJSON_AddItemToObject(root, "key_norm", child_num);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_before = cJSON_GetObjectItem(root, "key_cs");
    cJSON_DeleteItemFromObject(root, "key_cs");
    retrieved_after = cJSON_GetObjectItem(root, "key_cs");

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_cs + (int)added_norm + (int)(retrieved_before != (cJSON *)0) + (int)(retrieved_after == (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_before;
    (void)retrieved_after;
    (void)added_cs;
    (void)added_norm;
    // API sequence test completed successfully
    return 66;
}