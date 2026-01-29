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
//<ID> 798
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = NULL;
    cJSON *root = NULL;
    cJSON *raw_item = NULL;
    cJSON *int_array = NULL;
    char *printed = NULL;
    int arr_size = 0;
    int nums[3] = {1, 2, 3};
    int check_count = 0;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_CreateObject();
    int_array = cJSON_CreateIntArray(nums, 3);
    raw_item = cJSON_CreateRaw("{\"inner\":\"value\",\"flag\":true}");
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddItemToObject(root, "raw", raw_item);
    printed = cJSON_PrintUnformatted(root);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(int_array);
    check_count = (arr_size + (int)version[0]); /* use version pointer to influence data flow */

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}