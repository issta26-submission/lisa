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
//<ID> 2138
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *n3 = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_val = NULL;
    cJSON *got_first = NULL;
    cJSON_bool is_num_arr_first = 0;
    cJSON_bool is_num_val = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);
    n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "value", n3);

    // step 3: Operate
    got_arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    got_val = cJSON_GetObjectItemCaseSensitive(root, "value");
    got_first = cJSON_GetArrayItem(got_arr, 0);
    is_num_arr_first = cJSON_IsNumber(got_first);
    is_num_val = cJSON_IsNumber(got_val);
    val0 = cJSON_GetNumberValue(got_first);
    val1 = cJSON_GetNumberValue(got_val);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(n3 != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(got_val != NULL);
    validation ^= (int)(got_first != NULL);
    validation ^= (int)(is_num_arr_first != 0);
    validation ^= (int)(is_num_val != 0);
    validation ^= (int)(val0 == 10.0);
    validation ^= (int)(val1 == 30.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}