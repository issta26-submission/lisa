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
//<ID> 2115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *got_value = NULL;
    cJSON *got_flag = NULL;
    cJSON *replacement = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool is_true = 0;
    double new_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "value", 42.0);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate
    got_value = cJSON_GetObjectItemCaseSensitive(root, "value");
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    is_true = cJSON_IsTrue(got_flag);
    replacement = cJSON_CreateNumber(84.0);
    replaced = cJSON_ReplaceItemViaPointer(root, num_item, replacement);
    got_value = cJSON_GetObjectItemCaseSensitive(root, "value");
    new_val = cJSON_GetNumberValue(got_value);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(got_value != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(new_val == 84.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}