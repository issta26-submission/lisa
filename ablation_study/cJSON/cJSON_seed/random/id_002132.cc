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
//<ID> 2132
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
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_item = NULL;
    cJSON_bool is_num = 0;
    double extra_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.5);
    n2 = cJSON_CreateString("not_a_number");
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "extra", cJSON_CreateNumber(42.0));

    // step 3: Core operations
    got_arr = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    got_item = cJSON_GetArrayItem(got_arr, 1);
    is_num = cJSON_IsNumber(got_item);
    extra_val = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(root, "extra"));

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n0 != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(extra_val == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}