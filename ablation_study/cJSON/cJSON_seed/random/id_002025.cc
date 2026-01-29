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
//<ID> 2025
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
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *got1 = NULL;
    cJSON_bool added_n0 = 0;
    cJSON_bool added_n1 = 0;
    cJSON_bool added_arr = 0;
    double extracted = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(2.5);
    n2 = cJSON_CreateNumber(3.5);

    // step 3: Configure
    added_n0 = cJSON_AddItemToArray(arr, n0);
    added_n1 = cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddNumberToObject(root, "total", 7.5);
    cJSON_AddStringToObject(root, "name", "test");
    added_arr = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    got1 = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(got1);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n0 != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(added_n0 != 0);
    validation ^= (int)(added_n1 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(extracted == 2.5);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}