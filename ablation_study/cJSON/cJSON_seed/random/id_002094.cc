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
//<ID> 2094
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
    cJSON *num_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *got_array = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    double num_value = 0.0;
    cJSON_bool is_num = 0;
    cJSON_bool is_raw = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.14);
    raw_item = cJSON_CreateRaw("null");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Operate
    got_array = cJSON_GetObjectItem(root, "data");
    got0 = cJSON_GetArrayItem(got_array, 0);
    got1 = cJSON_GetArrayItem(got_array, 1);
    is_num = cJSON_IsNumber(got0);
    is_raw = cJSON_IsRaw(got1);
    num_value = cJSON_GetNumberValue(got0);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(raw_item != NULL);
    validation ^= (int)(got_array != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(is_raw != 0);
    validation ^= (int)(num_value == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: API sequence test completed successfully
    return 66;
}