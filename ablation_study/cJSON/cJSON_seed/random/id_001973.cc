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
//<ID> 1973
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *flag = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_first = NULL;
    double first_val = 0.0;
    int arr_size = 0;
    cJSON_bool is_array = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Core operations
    got_arr = cJSON_GetObjectItem(root, "values");
    is_array = cJSON_IsArray(got_arr);
    arr_size = cJSON_GetArraySize(got_arr);
    got_first = cJSON_GetArrayItem(got_arr, 0);
    first_val = cJSON_GetNumberValue(got_first);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(flag != NULL);
    validation ^= (int)(is_array != 0);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(first_val == 1.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}