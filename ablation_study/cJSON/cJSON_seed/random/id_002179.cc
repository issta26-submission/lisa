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
//<ID> 2179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *added_bool_item = NULL;
    cJSON *got_true = NULL;
    cJSON *got_flag = NULL;
    char *printed = NULL;
    cJSON_bool added_true = 0;
    cJSON_bool added_false = 0;
    cJSON_bool arr_check = 0;
    int validation = 0;

    // step 2: Setup (initialize object and create boolean items)
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    added_true = cJSON_AddItemToObject(root, "t_item", true_item);
    added_false = cJSON_AddItemToObject(root, "f_item", false_item);
    added_bool_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Core operations (serialize and inspect items)
    printed = cJSON_PrintUnformatted(root);
    got_true = cJSON_GetObjectItem(root, "t_item");
    got_flag = cJSON_GetObjectItem(root, "flag");
    arr_check = cJSON_IsArray(got_flag);

    // step 4: Validate (accumulate simple checks into validation)
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_true != 0);
    validation ^= (int)(added_false != 0);
    validation ^= (int)(added_bool_item != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(arr_check == 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}