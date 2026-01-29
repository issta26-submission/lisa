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
//<ID> 2137
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
    cJSON *name = NULL;
    cJSON *got_numbers = NULL;
    cJSON *first_num = NULL;
    cJSON_bool is_num = 0;
    double first_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(42.0);
    n2 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate
    got_numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    first_num = cJSON_GetArrayItem(got_numbers, 0);
    is_num = cJSON_IsNumber(first_num);
    first_val = cJSON_GetNumberValue(first_num);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(name != NULL);
    validation ^= (int)(got_numbers != NULL);
    validation ^= (int)(first_num != NULL);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(first_val == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}