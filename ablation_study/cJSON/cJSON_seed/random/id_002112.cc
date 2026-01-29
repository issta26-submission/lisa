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
//<ID> 2112
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
    cJSON *got_item = NULL;
    cJSON *replacement = NULL;
    cJSON *true_item = NULL;
    double got_value = 0.0;
    cJSON_bool true_flag = 0;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate
    got_item = cJSON_GetObjectItemCaseSensitive(root, "count");
    true_flag = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(root, "enabled"));
    replacement = cJSON_CreateNumber(100.0);
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, replacement);
    got_item = cJSON_GetObjectItemCaseSensitive(root, "count");
    got_value = cJSON_GetNumberValue(got_item);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(true_flag != 0);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(got_value == 100.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}