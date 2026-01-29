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
//<ID> 2032
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
    cJSON *label = NULL;
    cJSON *arr_entry = NULL;
    cJSON *got_label = NULL;
    cJSON *got_arr = NULL;
    char *old_value = NULL;
    cJSON_bool added_arrentry = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_label = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    label = cJSON_CreateString("initial_label");
    arr_entry = cJSON_CreateString("entry1");

    // step 3: Configure
    added_arrentry = cJSON_AddItemToArray(arr, arr_entry);
    added_arr_to_obj = cJSON_AddItemToObject(root, "data", arr);
    added_label = cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate / Inspect
    old_value = cJSON_SetValuestring(label, "updated_label");
    got_label = cJSON_GetObjectItem(root, "label");
    got_arr = cJSON_GetObjectItem(root, "data");
    arr_size = cJSON_GetArraySize(got_arr);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(label != NULL);
    validation ^= (int)(arr_entry != NULL);
    validation ^= (int)(got_label != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(old_value != NULL);
    validation ^= (int)(arr_size == 1);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}