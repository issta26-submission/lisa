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
//<ID> 1862
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
    cJSON *num_item = NULL;
    cJSON *dup_num = NULL;
    cJSON *arr_item = NULL;
    cJSON *retrieved_obj = NULL;
    cJSON *added_num_to_obj = NULL;
    char *out = NULL;
    double got_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num_item);
    added_num_to_obj = cJSON_AddNumberToObject(root, "pi_obj", 3.14);

    // step 3: Core operations
    dup_num = cJSON_Duplicate(added_num_to_obj, 1);
    cJSON_AddItemToArray(arr, dup_num);
    arr_item = cJSON_GetArrayItem(arr, 1);
    retrieved_obj = cJSON_GetObjectItem(root, "pi_obj");
    got_val = cJSON_GetNumberValue(arr_item);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(dup_num != NULL);
    validation ^= (int)(arr_item != NULL);
    validation ^= (int)(retrieved_obj != NULL);
    validation ^= (int)(added_num_to_obj != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_val == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}