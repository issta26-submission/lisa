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
//<ID> 1117
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
    cJSON *fetched_num_item = (cJSON *)0;
    cJSON *fetched_true_item = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_num = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object and a true boolean, add a number via helper
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate - fetch members and extract values
    fetched_num_item = cJSON_GetObjectItem(root, "answer");
    fetched_true_item = cJSON_GetObjectItem(root, "flag");
    retrieved_num = cJSON_GetNumberValue(fetched_num_item);

    // step 4: Validate - serialize briefly and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(fetched_num_item != (cJSON *)0) + (int)(fetched_true_item != (cJSON *)0) + (int)(retrieved_num == 42.0);
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 5: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}