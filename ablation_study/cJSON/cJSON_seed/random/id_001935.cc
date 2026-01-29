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
//<ID> 1935
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *newnum = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON *enabled = NULL;
    char *out = NULL;
    double set_ret = 0.0;
    double val0 = 0.0;
    double val1 = 0.0;
    cJSON_bool true_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Core operations
    newnum = cJSON_CreateNumber(3.0);
    cJSON_ReplaceItemInArray(arr, 1, newnum);
    set_ret = cJSON_SetNumberHelper(num0, 9.5);
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(got0);
    val1 = cJSON_GetNumberValue(got1);
    enabled = cJSON_GetObjectItem(root, "enabled");
    true_flag = cJSON_IsTrue(enabled);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num0 != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(newnum != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(true_flag != 0);
    validation ^= (int)(val0 == 9.5);
    validation ^= (int)(val1 == 3.0);
    validation ^= (int)(set_ret == 9.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}