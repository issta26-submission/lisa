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
//<ID> 2031
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
    cJSON *num = NULL;
    cJSON *arr_elem1 = NULL;
    cJSON *arr_elem2 = NULL;
    cJSON *got_name = NULL;
    cJSON_bool added_arr = 0;
    cJSON_bool added_name = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_elem1 = 0;
    cJSON_bool added_elem2 = 0;
    char *newval = NULL;
    char *extracted_name = NULL;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString("initial");
    num = cJSON_CreateNumber(7.0);
    arr_elem1 = cJSON_CreateNumber(1.0);
    arr_elem2 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "list", arr);
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_num = cJSON_AddItemToObject(root, "value", num);
    added_elem1 = cJSON_AddItemToArray(arr, arr_elem1);
    added_elem2 = cJSON_AddItemToArray(arr, arr_elem2);

    // step 4: Operate / Inspect
    newval = cJSON_SetValuestring(name_item, "updated");
    got_name = cJSON_GetObjectItem(root, "name");
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(name_item != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(arr_elem1 != NULL);
    validation ^= (int)(arr_elem2 != NULL);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_name != 0);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_elem1 != 0);
    validation ^= (int)(added_elem2 != 0);
    validation ^= (int)(got_name != NULL);
    validation ^= (int)(newval != NULL);
    validation ^= (int)(arr_size == 2);
    extracted_name = cJSON_GetStringValue(got_name);
    validation ^= (int)(extracted_name != NULL);
    validation ^= (int)(extracted_name[0] == 'u');
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}