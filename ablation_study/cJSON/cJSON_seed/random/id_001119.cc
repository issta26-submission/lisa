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
//<ID> 1119
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
    cJSON *true_item = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_status = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_num = 0.0;
    cJSON_bool is_num = (cJSON_bool)0;
    cJSON_bool is_status_true = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "status", true_item);

    // step 3: Operate - retrieve items
    fetched_num = cJSON_GetObjectItem(root, "count");
    fetched_status = cJSON_GetObjectItem(root, "status");

    // step 4: Validate - inspect types and values, serialize
    is_num = cJSON_IsNumber(fetched_num);
    retrieved_num = cJSON_GetNumberValue(fetched_num);
    is_status_true = cJSON_IsTrue(fetched_status);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)is_num + (int)(retrieved_num == 42.0) + (int)is_status_true;
    (void)validation_score;

    // step 5: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}