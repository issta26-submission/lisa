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
//<ID> 2023
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
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *pi_item = NULL;
    cJSON *name_item = NULL;
    cJSON *got1 = NULL;
    cJSON_bool addedA = 0;
    cJSON_bool addedB = 0;
    cJSON_bool addedArr = 0;
    double extracted = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_a = cJSON_CreateNumber(1.25);
    num_b = cJSON_CreateNumber(2.75);

    // step 3: Configure
    addedA = cJSON_AddItemToArray(arr, num_a);
    addedB = cJSON_AddItemToArray(arr, num_b);
    addedArr = cJSON_AddItemToObject(root, "list", arr);
    pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);
    name_item = cJSON_AddStringToObject(root, "name", "cJSON_test");

    // step 4: Operate / Inspect
    got1 = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(got1);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_a != NULL);
    validation ^= (int)(num_b != NULL);
    validation ^= (int)(addedA != 0);
    validation ^= (int)(addedB != 0);
    validation ^= (int)(addedArr != 0);
    validation ^= (int)(pi_item != NULL);
    validation ^= (int)(name_item != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(extracted == 2.75);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}