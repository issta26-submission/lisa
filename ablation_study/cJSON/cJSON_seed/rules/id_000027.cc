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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *num_old = cJSON_CreateNumber(1.0);
    cJSON *num_new = cJSON_CreateNumber(2.0);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *retr_flag = NULL;
    cJSON *retr_value = NULL;
    char *printed = NULL;
    cJSON_bool flag_is_true = 0;
    double value_after_replace = 0.0;
    int validation = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num_old);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, num_old, num_new);
    retr_flag = cJSON_GetObjectItem(root, "flag");
    flag_is_true = cJSON_IsTrue(retr_flag);
    retr_value = cJSON_GetObjectItem(root, "value");
    value_after_replace = cJSON_GetNumberValue(retr_value);
    printed = cJSON_PrintUnformatted(root);
    validation = (int)flag_is_true + (int)(value_after_replace == 2.0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation;
    return 66;
}