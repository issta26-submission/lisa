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
//<ID> 2187
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
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *got_arr = NULL;
    char *prebuf = NULL;
    cJSON_bool added_array = 0;
    cJSON_bool print_ok = 0;
    cJSON_bool is_array = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup (initialize object and array with items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("hello");
    bool_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, bool_item);
    added_array = cJSON_AddItemToObject(root, "myarray", arr);

    // step 3: Operate (allocate buffer, print, and inspect the array)
    prebuf = (char *)cJSON_malloc(128);
    print_ok = cJSON_PrintPreallocated(root, prebuf, 128, 0);
    got_arr = cJSON_GetObjectItem(root, "myarray");
    is_array = cJSON_IsArray(got_arr);
    arr_size = cJSON_GetArraySize(got_arr);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_array != 0);
    validation ^= (int)(prebuf != NULL);
    validation ^= (int)(print_ok != 0);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(is_array != 0);
    validation ^= (int)(arr_size == 3);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(prebuf);

    // API sequence test completed successfully
    return 66;
}