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
//<ID> 1937
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
    char *out = NULL;
    double set_ret = 0.0;
    double got_val = 0.0;
    cJSON_bool replaced = 0;
    cJSON_bool has_enabled = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddTrueToObject(root, "enabled");
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    newnum = cJSON_CreateNumber(0.0);
    set_ret = cJSON_SetNumberHelper(newnum, 42.0);
    replaced = cJSON_ReplaceItemInArray(arr, 0, newnum);
    cJSON_free(out);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got0 = cJSON_GetArrayItem(arr, 0);
    got_val = cJSON_GetNumberValue(got0);
    has_enabled = cJSON_HasObjectItem(root, "enabled");
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num0 != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(newnum != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(set_ret == 42.0);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got_val == 42.0);
    validation ^= (int)(has_enabled != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}