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
//<ID> 2118
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
    cJSON *orig_num = NULL;
    cJSON *replacement_num = NULL;
    cJSON *flag_item = NULL;
    cJSON *got_a_before = NULL;
    cJSON *got_a_after = NULL;
    cJSON *got_flag = NULL;
    cJSON_bool is_true = 0;
    cJSON_bool replaced = 0;
    double a_val = 0.0;
    int validation = 0;

    // step 2: Setup (Initialize and configure JSON)
    root = cJSON_CreateObject();
    orig_num = cJSON_AddNumberToObject(root, "a", 1.0);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);
    replacement_num = cJSON_CreateNumber(2.0);

    // step 3: Operate (Replace item and query)
    got_a_before = cJSON_GetObjectItemCaseSensitive(root, "a");
    replaced = cJSON_ReplaceItemViaPointer(root, got_a_before, replacement_num);
    got_a_after = cJSON_GetObjectItemCaseSensitive(root, "a");
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    is_true = cJSON_IsTrue(got_flag);
    a_val = cJSON_GetNumberValue(got_a_after);

    // step 4: Validate (accumulate simple checks without branches)
    validation ^= (int)(root != NULL);
    validation ^= (int)(orig_num != NULL);
    validation ^= (int)(replacement_num != NULL);
    validation ^= (int)(got_a_before != NULL);
    validation ^= (int)(got_a_after != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(a_val == 2.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}