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
//<ID> 2130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *pi = NULL;
    cJSON *got_numbers = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_elem0 = NULL;
    cJSON *got_elem1 = NULL;
    cJSON_bool is_num = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(42.0);
    n2 = cJSON_CreateNumber(3.14);
    pi = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "pi", pi);

    // step 3: Operate
    got_numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    got_pi = cJSON_GetObjectItemCaseSensitive(root, "pi");
    got_elem0 = cJSON_GetArrayItem(got_numbers, 0);
    got_elem1 = cJSON_GetArrayItem(got_numbers, 1);
    v0 = cJSON_GetNumberValue(got_elem0);
    v1 = cJSON_GetNumberValue(got_elem1);
    is_num = cJSON_IsNumber(got_elem1);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(pi != NULL);
    validation ^= (int)(got_numbers != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_elem0 != NULL);
    validation ^= (int)(got_elem1 != NULL);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(v0 == 42.0);
    validation ^= (int)(v1 == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}