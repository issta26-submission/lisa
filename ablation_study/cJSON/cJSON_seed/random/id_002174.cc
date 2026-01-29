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
//<ID> 2174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[48];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *created_true = NULL;
    cJSON *created_false = NULL;
    cJSON *added_true_item = NULL;
    cJSON *added_false_item = NULL;
    cJSON *arr = NULL;
    cJSON *got_arr = NULL;
    char *printed = NULL;
    cJSON_bool is_array_flag = 0;
    cJSON_bool added_array = 0;
    int validation = 0;

    // step 2: Setup (create root and standalone boolean objects)
    root = cJSON_CreateObject();
    created_true = cJSON_CreateTrue();
    created_false = cJSON_CreateFalse();

    // step 3: Configure (add boolean members using AddBoolToObject and create an array)
    added_true_item = cJSON_AddBoolToObject(root, "enabled", 1);
    added_false_item = cJSON_AddBoolToObject(root, "disabled", 0);
    arr = cJSON_CreateArray();
    added_array = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate (inspect and serialize)
    got_arr = cJSON_GetObjectItem(root, "items");
    is_array_flag = cJSON_IsArray(got_arr);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate (accumulate simple checks into validation)
    validation ^= (int)(root != NULL);
    validation ^= (int)(created_true != NULL);
    validation ^= (int)(created_false != NULL);
    validation ^= (int)(added_true_item != NULL);
    validation ^= (int)(added_false_item != NULL);
    validation ^= (int)(added_array != 0);
    validation ^= (int)(is_array_flag != 0);
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(created_true);
    cJSON_Delete(created_false);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}