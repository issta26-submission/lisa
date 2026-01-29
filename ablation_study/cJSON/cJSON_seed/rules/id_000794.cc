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
//<ID> 794
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *raw_item = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *count_num = NULL;
    char *printed = NULL;
    const char *version = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "numbers", arr);
    raw_item = cJSON_CreateRaw("[true, false, null]");
    cJSON_AddItemToObject(root, "raw_data", raw_item);
    printed = cJSON_PrintUnformatted(root);
    version = cJSON_Version();

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    count_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}