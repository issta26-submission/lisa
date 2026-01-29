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
//<ID> 1979
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
    cJSON *num_item = NULL;
    cJSON *false_item = NULL;
    cJSON *got_num_item = NULL;
    cJSON_bool added_num = 0;
    cJSON_bool added_false = 0;
    cJSON_bool is_array = 0;
    int arr_size = 0;
    double num_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.14);
    false_item = cJSON_CreateFalse();

    // step 3: Core operations
    added_num = cJSON_AddItemToArray(root, num_item);
    added_false = cJSON_AddItemToArray(root, false_item);
    arr_size = cJSON_GetArraySize(root);
    is_array = cJSON_IsArray(root);
    got_num_item = cJSON_GetArrayItem(root, 0);
    num_val = cJSON_GetNumberValue(got_num_item);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_false != 0);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(is_array != 0);
    validation ^= (int)(num_val == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}