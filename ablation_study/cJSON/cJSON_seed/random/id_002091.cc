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
//<ID> 2091
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
    cJSON *raw_item = NULL;
    cJSON *array = NULL;
    cJSON *arr_num = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_blob = NULL;
    cJSON *got_arr = NULL;
    cJSON *arr_item0 = NULL;
    double arr_value = 0.0;
    cJSON_bool is_pi_number = 0;
    cJSON_bool is_blob_raw = 0;
    cJSON_bool is_arr_item_number = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14);
    raw_item = cJSON_CreateRaw("unparsed: {\"x\":1}");
    array = cJSON_CreateArray();
    arr_num = cJSON_CreateNumber(7.0);

    // step 3: Configure
    cJSON_AddItemToArray(array, arr_num);
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON_AddItemToObject(root, "blob", raw_item);
    cJSON_AddItemToObject(root, "arr", array);

    // step 4: Operate
    got_pi = cJSON_GetObjectItem(root, "pi");
    got_blob = cJSON_GetObjectItem(root, "blob");
    got_arr = cJSON_GetObjectItem(root, "arr");
    is_pi_number = cJSON_IsNumber(got_pi);
    is_blob_raw = cJSON_IsRaw(got_blob);
    arr_item0 = cJSON_GetArrayItem(got_arr, 0);
    arr_value = cJSON_GetNumberValue(arr_item0);
    is_arr_item_number = cJSON_IsNumber(arr_item0);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(raw_item != NULL);
    validation ^= (int)(array != NULL);
    validation ^= (int)(arr_num != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_blob != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(is_pi_number != 0);
    validation ^= (int)(is_blob_raw != 0);
    validation ^= (int)(arr_item0 != NULL);
    validation ^= (int)(is_arr_item_number != 0);
    validation ^= (int)(arr_value == 7.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}