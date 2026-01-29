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
//<ID> 2131
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
    cJSON *direct_num = NULL;
    cJSON *got_numbers_obj = NULL;
    cJSON *got_array_item = NULL;
    cJSON_bool is_array_item_number = 0;
    cJSON_bool is_direct_number = 0;
    double direct_value = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(42.0);
    n2 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "direct", cJSON_CreateNumber(3.14));

    // step 3: Operate
    got_numbers_obj = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    got_array_item = cJSON_GetArrayItem(got_numbers_obj, 1);
    is_array_item_number = cJSON_IsNumber(got_array_item);
    direct_num = cJSON_GetObjectItemCaseSensitive(root, "direct");
    is_direct_number = cJSON_IsNumber(direct_num);
    direct_value = cJSON_GetNumberValue(direct_num);
    buf[0] = (char)(is_array_item_number + '0');

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(got_numbers_obj != NULL);
    validation ^= (int)(got_array_item != NULL);
    validation ^= (int)(is_array_item_number != 0);
    validation ^= (int)(is_direct_number != 0);
    validation ^= (int)(direct_value == 3.14);
    buf[1] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}