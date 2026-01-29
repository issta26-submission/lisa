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
//<ID> 2183
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *got_arr = NULL;
    char *printed = NULL;
    void *tmp_alloc = NULL;
    cJSON_bool is_array = 0;
    int arr_size = 0;
    int validation = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr = 0;

    // step 2: Setup (initialize object, create array and number items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.5);
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Core operations (serialize, retrieve array, inspect type/size, allocate memory)
    printed = cJSON_PrintUnformatted(root);
    got_arr = cJSON_GetObjectItem(root, "numbers");
    is_array = cJSON_IsArray(got_arr);
    arr_size = cJSON_GetArraySize(got_arr);
    tmp_alloc = cJSON_malloc((size_t)(arr_size + 8));
    memset(tmp_alloc, 0, (size_t)(arr_size + 8));

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(is_array != 0);
    validation ^= (int)(arr_size == 2);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    cJSON_free(tmp_alloc);

    // API sequence test completed successfully
    return 66;
}