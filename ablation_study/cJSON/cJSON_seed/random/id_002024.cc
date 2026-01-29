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
//<ID> 2024
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON *label_item = NULL;
    cJSON *direct_item = NULL;
    cJSON *direct_lookup = NULL;
    double extracted_from_array = 0.0;
    double extracted_direct = 0.0;
    cJSON_bool added_arr = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    int validation = 0;

    // Step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);

    // Step 3: Configure
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    label_item = cJSON_AddStringToObject(root, "label", "test");
    direct_item = cJSON_AddNumberToObject(root, "direct", 100.0);

    // Step 4: Operate / Inspect
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    extracted_from_array = cJSON_GetNumberValue(got1);
    direct_lookup = cJSON_GetObjectItem(root, "direct");
    extracted_direct = cJSON_GetNumberValue(direct_lookup);

    // Step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(label_item != NULL);
    validation ^= (int)(direct_item != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(extracted_from_array == 2.5);
    validation ^= (int)(extracted_direct == 100.0);
    buf[0] = (char)(validation + '0');

    // Step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}