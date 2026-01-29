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
//<ID> 2092
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
    cJSON *array = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *raw_item = NULL;
    cJSON *got_array = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    double got_value0 = 0.0;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_raw1 = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(42.0);
    num1 = cJSON_CreateNumber(7.0);
    raw_item = cJSON_CreateRaw("raw_payload");
    cJSON_AddItemToArray(array, num0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, raw_item);
    cJSON_AddItemToObject(root, "values", array);

    // step 3: Operate
    got_array = cJSON_GetObjectItem(root, "values");
    got0 = cJSON_GetArrayItem(got_array, 0);
    got1 = cJSON_GetArrayItem(got_array, 2);
    got_value0 = cJSON_GetNumberValue(got0);
    is_num0 = cJSON_IsNumber(got0);
    is_raw1 = cJSON_IsRaw(got1);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(array != NULL);
    validation ^= (int)(num0 != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(raw_item != NULL);
    validation ^= (int)(got_array != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(got_value0 == 42.0);
    validation ^= (int)(is_num0 != 0);
    validation ^= (int)(is_raw1 != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}