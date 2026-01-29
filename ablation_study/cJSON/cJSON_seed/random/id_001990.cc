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
//<ID> 1990
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
    cJSON *arr = NULL;
    cJSON *name_item = NULL;
    cJSON *age_item = NULL;
    cJSON *arr_item = NULL;
    cJSON *got_name = NULL;
    cJSON *got_items = NULL;
    char *json_str = NULL;
    const char *name_value = NULL;
    cJSON_bool added_name = 0;
    cJSON_bool added_age = 0;
    cJSON_bool added_items = 0;
    cJSON_bool added_arr_item = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString("alice");
    age_item = cJSON_CreateString("30");
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_age = cJSON_AddItemToObject(root, "age", age_item);
    added_items = cJSON_AddItemToObject(root, "items", arr);

    // step 3: Core operations
    arr_item = cJSON_CreateString("first");
    added_arr_item = cJSON_AddItemToArray(arr, arr_item);
    json_str = cJSON_PrintUnformatted(root);
    got_name = cJSON_GetObjectItem(root, "name");
    got_items = cJSON_GetObjectItem(root, "items");
    name_value = (got_name != NULL) ? cJSON_GetStringValue(got_name) : NULL;

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(name_item != NULL);
    validation ^= (int)(age_item != NULL);
    validation ^= (int)(arr_item != NULL);
    validation ^= (int)(added_name != 0);
    validation ^= (int)(added_age != 0);
    validation ^= (int)(added_items != 0);
    validation ^= (int)(added_arr_item != 0);
    validation ^= (int)(json_str != NULL);
    validation ^= (int)(got_name != NULL);
    validation ^= (int)(got_items != NULL);
    validation ^= (int)(name_value != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free((void *)json_str);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}