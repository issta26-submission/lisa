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
//<ID> 1931
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *newnum = NULL;
    char *out = NULL;
    double set_ret = 0.0;
    double got_val = 0.0;
    int arr_size = 0;
    cJSON_bool replaced_ok = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Core operations
    newnum = cJSON_CreateNumber(42.0);
    replaced_ok = cJSON_ReplaceItemInArray(arr, 1, newnum);
    set_ret = cJSON_SetNumberHelper(newnum, 100.5);
    out = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);
    got_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(newnum != NULL);
    validation ^= (int)(replaced_ok != 0);
    validation ^= (int)(set_ret == 100.5);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(got_val == 100.5);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}