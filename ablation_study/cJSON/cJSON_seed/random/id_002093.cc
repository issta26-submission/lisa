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
//<ID> 2093
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
    cJSON *vals_array = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num3 = NULL;
    cJSON *raw_item = NULL;
    cJSON *got_array = NULL;
    cJSON *got_item = NULL;
    cJSON *got_raw = NULL;
    double got_val = 0.0;
    cJSON_bool is_num = 0;
    cJSON_bool is_raw = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    vals_array = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    num3 = cJSON_CreateNumber(3.5);
    raw_item = cJSON_CreateRaw("RAW_DATA");
    cJSON_AddItemToObject(root, "pi", cJSON_CreateNumber(3.14159));

    // step 3: Configure
    cJSON_AddItemToArray(vals_array, num1);
    cJSON_AddItemToArray(vals_array, num2);
    cJSON_AddItemToArray(vals_array, num3);
    cJSON_AddItemToObject(root, "values", vals_array);
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 4: Operate
    got_array = cJSON_GetObjectItem(root, "values");
    got_item = cJSON_GetArrayItem(got_array, 2);
    got_val = cJSON_GetNumberValue(got_item);
    is_num = cJSON_IsNumber(got_item);
    got_raw = cJSON_GetObjectItem(root, "raw");
    is_raw = cJSON_IsRaw(got_raw);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(vals_array != NULL);
    validation ^= (int)(got_array != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(got_val == 3.5);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(is_raw != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}