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
//<ID> 2119
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
    cJSON *num_orig = NULL;
    cJSON *num_new = NULL;
    cJSON *got_num = NULL;
    cJSON *flag_item = NULL;
    cJSON *got_flag = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool is_true = 0;
    double got_value = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_orig = cJSON_AddNumberToObject(root, "a", 1.0);
    num_new = cJSON_CreateNumber(2.0);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate
    replaced = cJSON_ReplaceItemViaPointer(root, num_orig, num_new);
    got_num = cJSON_GetObjectItemCaseSensitive(root, "a");
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    is_true = cJSON_IsTrue(got_flag);
    got_value = cJSON_GetNumberValue(got_num);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_orig != NULL);
    validation ^= (int)(num_new != NULL);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(got_value == 2.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}