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
//<ID> 2037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *greet = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_greet = NULL;
    cJSON *first_in_arr = NULL;
    char *set_ret = NULL;
    int arr_size = 0;
    cJSON_bool added_s1 = 0;
    cJSON_bool added_s2 = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_greet = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    greet = cJSON_CreateString("hi");

    // step 3: Configure
    added_s1 = cJSON_AddItemToArray(arr, s1);
    added_s2 = cJSON_AddItemToArray(arr, s2);
    added_arr = cJSON_AddItemToObject(root, "items", arr);
    added_greet = cJSON_AddItemToObject(root, "greeting", greet);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(got_arr);
    first_in_arr = cJSON_GetArrayItem(got_arr, 0);
    got_greet = cJSON_GetObjectItem(root, "greeting");
    set_ret = cJSON_SetValuestring(got_greet, "hello world");

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(s1 != NULL);
    validation ^= (int)(s2 != NULL);
    validation ^= (int)(greet != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(got_greet != NULL);
    validation ^= (int)(first_in_arr != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(set_ret != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}