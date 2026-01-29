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
//<ID> 2185
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
    void *mem = NULL;
    cJSON_bool is_arr = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup (initialize object and populate an array)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Configure (allocate helper memory and serialize)
    mem = cJSON_malloc(32);
    memset(mem, 0, 32);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Operate (retrieve object item and inspect as array)
    got_arr = cJSON_GetObjectItem(root, "values");
    arr_size = cJSON_GetArraySize(got_arr);
    is_arr = cJSON_IsArray(got_arr);

    // step 5: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(is_arr != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    cJSON_free(mem);

    // API sequence test completed successfully
    return 66;
}