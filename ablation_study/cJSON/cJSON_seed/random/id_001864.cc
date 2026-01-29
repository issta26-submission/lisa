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
//<ID> 1864
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
    cJSON *num_in_obj = NULL;
    cJSON *num_in_array = NULL;
    cJSON *dup_item = NULL;
    cJSON *retrieved_from_array = NULL;
    cJSON *retrieved_from_object = NULL;
    char *out = NULL;
    double val_from_array = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    num_in_obj = cJSON_AddNumberToObject(root, "pi", 3.1415);
    num_in_array = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(arr, num_in_array);

    // step 3: Core operations
    dup_item = cJSON_Duplicate(num_in_obj, 0);
    cJSON_AddItemToArray(arr, dup_item);
    retrieved_from_array = cJSON_GetArrayItem(arr, 1);
    retrieved_from_object = cJSON_GetObjectItem(root, "pi");
    val_from_array = cJSON_GetNumberValue(retrieved_from_array);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_in_obj != NULL);
    validation ^= (int)(num_in_array != NULL);
    validation ^= (int)(dup_item != NULL);
    validation ^= (int)(retrieved_from_array != NULL);
    validation ^= (int)(retrieved_from_object != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(val_from_array == 3.1415);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}