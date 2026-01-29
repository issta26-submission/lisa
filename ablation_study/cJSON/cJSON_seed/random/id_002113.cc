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
//<ID> 2113
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
    cJSON *got_num = NULL;
    cJSON *true_item = NULL;
    cJSON *replacement_num = NULL;
    cJSON *got_flag = NULL;
    cJSON_bool is_true = 0;
    cJSON_bool replaced = 0;
    double num_val = 0.0;
    double new_num = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "k", 42.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate
    got_num = cJSON_GetObjectItemCaseSensitive(root, "k");
    num_val = cJSON_GetNumberValue(got_num);
    is_true = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(root, "flag"));
    replacement_num = cJSON_CreateNumber(7.0);
    replaced = cJSON_ReplaceItemViaPointer(root, true_item, replacement_num);
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    new_num = cJSON_GetNumberValue(got_flag);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(num_val == 42.0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(new_num == 7.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}