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
//<ID> 1036
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *darray = NULL;
    cJSON *active_flag = NULL;
    cJSON *got_measurements = NULL;
    cJSON *first_val = NULL;
    char *printed = NULL;
    const double nums[3] = {3.14, 2.71, 1.62};
    double first_number = 0.0;
    int array_size = 0;
    cJSON_bool is_active_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    active_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", active_flag);
    darray = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "measurements", darray);

    // step 3: Operate / Validate
    got_measurements = cJSON_GetObjectItemCaseSensitive(root, "measurements");
    array_size = cJSON_GetArraySize(got_measurements);
    first_val = cJSON_GetArrayItem(got_measurements, 0);
    first_number = cJSON_GetNumberValue(first_val);
    is_active_false = cJSON_IsFalse(cJSON_GetObjectItemCaseSensitive(root, "active"));

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)first_number;
    (void)array_size;
    (void)is_active_false;
    return 66;
}