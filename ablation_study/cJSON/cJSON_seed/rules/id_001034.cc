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
//<ID> 1034
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
    cJSON *flag = NULL;
    cJSON *got_array = NULL;
    cJSON *got_flag = NULL;
    char *printed = NULL;
    const double nums[3] = {0.5, 1.5, 2.5};
    int array_size = 0;
    double first_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    darray = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "measurements", darray);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate / Validate
    got_array = cJSON_GetObjectItemCaseSensitive(root, "measurements");
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "active");
    array_size = cJSON_GetArraySize(got_array);
    first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(got_array, 0));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}