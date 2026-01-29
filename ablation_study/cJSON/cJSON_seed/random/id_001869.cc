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
//<ID> 1869
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
    cJSON *num_orig = NULL;
    cJSON *num_added = NULL;
    cJSON *dup_num = NULL;
    cJSON *arr_item = NULL;
    cJSON *got_from_arr = NULL;
    cJSON *got_from_obj = NULL;
    char *out = NULL;
    double val_from_arr = 0.0;
    double val_from_obj = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "my_array", arr);
    num_orig = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "original_number", num_orig);
    num_added = cJSON_AddNumberToObject(root, "added_number", 7.5);

    // step 3: Core operations
    dup_num = cJSON_Duplicate(num_orig, 1);
    cJSON_AddItemToArray(arr, dup_num);
    arr_item = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(arr, arr_item);
    got_from_arr = cJSON_GetArrayItem(arr, 0);
    got_from_obj = cJSON_GetObjectItem(root, "original_number");
    val_from_arr = cJSON_GetNumberValue(got_from_arr);
    val_from_obj = cJSON_GetNumberValue(got_from_obj);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_orig != NULL);
    validation ^= (int)(num_added != NULL);
    validation ^= (int)(dup_num != NULL);
    validation ^= (int)(arr_item != NULL);
    validation ^= (int)(got_from_arr != NULL);
    validation ^= (int)(got_from_obj != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(val_from_arr == 42.0);
    validation ^= (int)(val_from_obj == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}