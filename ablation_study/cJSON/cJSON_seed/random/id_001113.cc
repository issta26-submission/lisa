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
//<ID> 1113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_true = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_num = 0.0;
    cJSON_bool retrieved_true = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a standalone true item
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();

    // step 3: Configure - add a numeric member to the root and attach the true item
    num_item = cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON_AddItemToObject(root, "active", true_item);

    // step 4: Operate - retrieve members and inspect their values
    fetched_num = cJSON_GetObjectItem(root, "count");
    fetched_true = cJSON_GetObjectItem(root, "active");
    retrieved_num = cJSON_GetNumberValue(fetched_num);
    retrieved_true = cJSON_IsTrue(fetched_true);

    // step 5: Validate - serialize and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_true != (cJSON *)0) + (int)(retrieved_num == 7.0) + (int)(retrieved_true != 0);
    (void)validation_score;

    // step 6: Cleanup - free serialized buffer and delete the root (frees attached items)
    if (printed != (char *)0) {
        cJSON_free((void *)printed);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}