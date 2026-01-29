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
//<ID> 1037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *measurements = NULL;
    cJSON *active = NULL;
    cJSON *got_measurements = NULL;
    cJSON *got_active = NULL;
    char *printed = NULL;
    const double nums[4] = {0.5, 1.5, 2.5, 3.5};
    double first_val = 0.0;
    int arr_size = 0;
    cJSON_bool is_active_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    measurements = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(root, "measurements", measurements);
    active = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", active);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("sensorA"));

    // step 3: Operate / Validate
    got_measurements = cJSON_GetObjectItemCaseSensitive(root, "measurements");
    arr_size = cJSON_GetArraySize(got_measurements);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(got_measurements, 0));
    got_active = cJSON_GetObjectItemCaseSensitive(root, "active");
    is_active_false = cJSON_IsFalse(got_active);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}