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
//<ID> 1038
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
    cJSON *got_measurements = NULL;
    cJSON *first_value = NULL;
    cJSON *got_flag = NULL;
    char *printed = NULL;
    const double nums[3] = {1.23, 4.56, 7.89};
    double extracted_value = 0.0;
    cJSON_bool is_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    darray = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "measurements", darray);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateString("unit-test"));

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    got_measurements = cJSON_GetObjectItemCaseSensitive(root, "measurements");
    first_value = cJSON_GetArrayItem(got_measurements, 0);
    extracted_value = cJSON_GetNumberValue(first_value);
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "active");
    is_false = cJSON_IsFalse(got_flag);
    (void)extracted_value;
    (void)is_false;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}