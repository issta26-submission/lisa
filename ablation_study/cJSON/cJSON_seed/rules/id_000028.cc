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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item_initial = cJSON_CreateNumber(42.0);
    cJSON *num_item_replacement = cJSON_CreateNumber(84.0);
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON *retr_old = NULL;
    cJSON *retr_new = NULL;
    cJSON *validation_num = NULL;
    double new_value = 0.0;
    int validation = 0;
    cJSON_bool flag_true = 0;

    // step 2: Configure
    cJSON_AddItemToObject(root, "answer", num_item_initial);
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate / Validate
    retr_old = cJSON_GetObjectItem(root, "answer");
    cJSON_ReplaceItemViaPointer(root, retr_old, num_item_replacement);
    retr_new = cJSON_GetObjectItem(root, "answer");
    new_value = cJSON_GetNumberValue(retr_new);
    flag_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    validation = (int)flag_true + (int)new_value;
    validation_num = cJSON_CreateNumber((double)validation);
    cJSON_AddItemToObject(root, "validation", validation_num);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}