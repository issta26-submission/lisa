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
//<ID> 1865
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
    cJSON *obj_num = NULL;
    cJSON *dup_num = NULL;
    cJSON *arr_item = NULL;
    char *out = NULL;
    double val_from_array = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddNumberToObject(root, "original", 42.0);

    // step 3: Core operations
    obj_num = cJSON_GetObjectItem(root, "original");
    dup_num = cJSON_Duplicate(obj_num, 0);
    cJSON_AddItemToArray(arr, dup_num);

    // step 4: Validate / extract
    arr_item = cJSON_GetArrayItem(arr, 0);
    val_from_array = cJSON_GetNumberValue(arr_item);
    out = cJSON_PrintUnformatted(root);
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(obj_num != NULL);
    validation ^= (int)(dup_num != NULL);
    validation ^= (int)(arr_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(val_from_array == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}