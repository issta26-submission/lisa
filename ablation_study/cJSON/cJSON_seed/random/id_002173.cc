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
//<ID> 2173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item_true = NULL;
    cJSON *item_false = NULL;
    cJSON *direct_bool = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_direct = NULL;
    char *printed = NULL;
    cJSON_bool added_arr = 0;
    cJSON_bool is_array = 0;
    int validation = 0;

    // step 2: Setup (Initialize root, create array and boolean items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_true = cJSON_CreateTrue();
    item_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, item_true);
    cJSON_AddItemToArray(arr, item_false);
    added_arr = cJSON_AddItemToObject(root, "arr", arr);
    direct_bool = cJSON_AddBoolToObject(root, "direct_flag", 1);

    // step 3: Operate (serialize and inspect)
    printed = cJSON_PrintUnformatted(root);
    got_arr = cJSON_GetObjectItem(root, "arr");
    is_array = cJSON_IsArray(got_arr);
    got_direct = cJSON_GetObjectItem(root, "direct_flag");

    // step 4: Validate (accumulate simple checks into validation)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(item_true != NULL);
    validation ^= (int)(item_false != NULL);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(direct_bool != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(is_array != 0);
    validation ^= (int)(got_direct != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}