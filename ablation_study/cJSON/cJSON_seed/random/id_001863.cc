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
//<ID> 1863
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
    cJSON *dup_num = NULL;
    cJSON *retrieved_arr_item = NULL;
    cJSON *retrieved_obj_item = NULL;
    char *out = NULL;
    double val_b = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    num1 = cJSON_AddNumberToObject(root, "a", 1.5);
    num2 = cJSON_AddNumberToObject(root, "b", 2.5);

    // step 3: Core operations
    dup_num = cJSON_Duplicate(num2, 1);
    cJSON_AddItemToArray(arr, dup_num);
    retrieved_arr_item = cJSON_GetArrayItem(arr, 0);
    retrieved_obj_item = cJSON_GetObjectItem(root, "b");
    val_b = cJSON_GetNumberValue(retrieved_obj_item);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(dup_num != NULL);
    validation ^= (int)(retrieved_arr_item != NULL);
    validation ^= (int)(retrieved_obj_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(val_b == 2.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}