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
//<ID> 1934
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *new_item = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    char *out = NULL;
    double setval_ret = 0.0;
    double val0 = 0.0;
    double val1 = 0.0;
    cJSON_bool replaced_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateNumber(1.0);
    item1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddTrueToObject(root, "flag");
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    new_item = cJSON_CreateNumber(42.0);
    replaced_flag = cJSON_ReplaceItemInArray(arr, 0, new_item);
    setval_ret = cJSON_SetNumberHelper(new_item, 99.5);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(got0);
    val1 = cJSON_GetNumberValue(got1);
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(item0 != NULL);
    validation ^= (int)(item1 != NULL);
    validation ^= (int)(new_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(replaced_flag != 0);
    validation ^= (int)(setval_ret == 99.5);
    validation ^= (int)(val0 == 99.5);
    validation ^= (int)(val1 == 2.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}