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
//<ID> 2030
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
    cJSON *str_item = NULL;
    cJSON *num_item = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_label = NULL;
    cJSON_bool added_num = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_label = 0;
    char *new_valuestr = NULL;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("initial");
    num_item = cJSON_CreateNumber(10.0);

    // step 3: Configure
    added_num = cJSON_AddItemToArray(arr, num_item);
    added_arr = cJSON_AddItemToObject(root, "list", arr);
    added_label = cJSON_AddItemToObject(root, "label", str_item);
    new_valuestr = cJSON_SetValuestring(str_item, "updated");

    // step 4: Operate / Inspect
    got_arr = cJSON_GetObjectItem(root, "list");
    got_label = cJSON_GetObjectItem(root, "label");
    arr_size = cJSON_GetArraySize(got_arr);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_label != 0);
    validation ^= (int)(new_valuestr != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(got_label != NULL);
    validation ^= (int)(arr_size == 1);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}