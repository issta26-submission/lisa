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
//<ID> 1257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const double numbers_arr[] = {1.5, 2.5, 3.5};
    const int numbers_count = 3;
    const char raw_meta[] = "{\"nested\":true}";
    cJSON *root = NULL;
    cJSON *nums = NULL;
    cJSON *second_item = NULL;
    char *printed = NULL;
    double extracted_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = cJSON_CreateDoubleArray(numbers_arr, numbers_count);
    cJSON_AddItemToObject(root, "numbers", nums);
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddRawToObject(root, "metadata", raw_meta);

    // step 3: Operate / Validate
    second_item = cJSON_GetArrayItem(nums, 1);
    extracted_value = cJSON_GetNumberValue(second_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)extracted_value;
    return 66;
}