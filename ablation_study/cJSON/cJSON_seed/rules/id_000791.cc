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
//<ID> 791
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *raw = NULL;
    cJSON *got_item = NULL;
    const char *version = NULL;
    int arr_size = 0;
    double value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.5);
    num2 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    raw = cJSON_CreateRaw("{\"note\":\"raw content\"}");
    cJSON_AddItemToObject(root, "raw_blob", raw);
    version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    got_item = cJSON_GetArrayItem(arr, 1);
    value = cJSON_GetNumberValue(got_item);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}