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
//<ID> 2020
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *n3 = NULL;
    cJSON *got1 = NULL;
    cJSON *added_count = NULL;
    cJSON *added_name = NULL;
    double extracted_value = 0.0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.1);
    n2 = cJSON_CreateNumber(2.2);
    n3 = cJSON_CreateNumber(3.3);

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, n1);
    added2 = cJSON_AddItemToArray(arr, n2);
    added_arr = cJSON_AddItemToObject(root, "items", arr);
    added_count = cJSON_AddNumberToObject(root, "count", 3.0);
    added_name = cJSON_AddStringToObject(root, "name", "fuzz");

    // step 4: Operate
    got1 = cJSON_GetArrayItem(arr, 1);
    extracted_value = cJSON_GetNumberValue(got1);
    cJSON_AddItemToArray(arr, n3);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(added_count != NULL);
    validation ^= (int)(added_name != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(extracted_value == 2.2);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}