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
//<ID> 722
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *active_item = (cJSON *)0;
    cJSON *score_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    const char *name_str = (const char *)0;
    double score_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("tester");
    active_item = cJSON_CreateFalse();
    score_item = cJSON_CreateNumber(99.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "active", active_item);
    cJSON_AddItemToObject(root, "score", score_item);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(retrieved_name);
    score_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "score"));

    // step 5: Validate
    validation_score = (int)(name_str != (const char *)0)
                     + (int)cJSON_IsFalse(cJSON_GetObjectItem(root, "active"))
                     + (int)(score_value > 0.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}