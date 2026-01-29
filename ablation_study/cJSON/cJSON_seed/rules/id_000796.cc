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
//<ID> 796
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = NULL;
    const char *raw_content = "[10,20,30]";
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num3 = NULL;
    cJSON *raw = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    num3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, num3);
    cJSON_AddItemToObject(root, "arr", arr);
    raw = cJSON_CreateRaw(raw_content);
    cJSON_AddItemToObject(root, "raw", raw);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    (void)version;
    (void)arr_size;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}