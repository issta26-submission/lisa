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
//<ID> 2184
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
    cJSON *meta = NULL;
    cJSON *got_arr = NULL;
    void *tmp = NULL;
    int arr_size = 0;
    cJSON_bool is_arr = 0;
    int validation = 0;

    // step 2: Setup (allocate temp string and build JSON structure)
    tmp = cJSON_malloc(8);
    ((char *)tmp)[0] = 'm';
    ((char *)tmp)[1] = 'e';
    ((char *)tmp)[2] = 't';
    ((char *)tmp)[3] = 'a';
    ((char *)tmp)[4] = 0;
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    meta = cJSON_CreateString((const char *)tmp);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate (retrieve object item and inspect array)
    got_arr = cJSON_GetObjectItem(root, "values");
    is_arr = cJSON_IsArray(got_arr);
    arr_size = cJSON_GetArraySize(got_arr);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(meta != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(is_arr != 0);
    validation ^= (int)(arr_size == 2);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(tmp);

    // API sequence test completed successfully
    return 66;
}