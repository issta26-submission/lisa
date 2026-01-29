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
//<ID> 1035
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *darr = NULL;
    cJSON *flag = NULL;
    cJSON *name = NULL;
    cJSON *got_data = NULL;
    cJSON *got_flag = NULL;
    char *printed = NULL;
    const double nums[4] = {0.1, 2.5, -3.14, 42.0};
    int arr_size = 0;
    cJSON_bool is_flag_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    darr = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(root, "data", darr);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "valid", flag);
    name = cJSON_CreateString("deviceX");
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate / Validate
    got_data = cJSON_GetObjectItemCaseSensitive(root, "data");
    arr_size = cJSON_GetArraySize(got_data);
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "valid");
    is_flag_false = cJSON_IsFalse(got_flag);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}