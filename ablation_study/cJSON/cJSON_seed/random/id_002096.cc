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
//<ID> 2096
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
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *raw_item = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_num = NULL;
    cJSON *got_raw = NULL;
    double got_value = 0.0;
    cJSON_bool is_num = 0;
    cJSON_bool is_raw = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(42.0);
    raw_item = cJSON_CreateRaw("example_raw");
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate
    got_arr = cJSON_GetObjectItem(root, "values");
    got_raw = cJSON_GetObjectItem(root, "raw");
    got_num = cJSON_GetArrayItem(got_arr, 1);
    is_num = cJSON_IsNumber(got_num);
    got_value = cJSON_GetNumberValue(got_num);
    is_raw = cJSON_IsRaw(got_raw);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num0 != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(raw_item != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(got_raw != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(is_raw != 0);
    validation ^= (int)(got_value == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}