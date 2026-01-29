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
//<ID> 1250
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const double nums[] = {3.14, 2.718, -1.0};
    const char raw_json[] = "{\"inner\":42}";
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *arr_item = NULL;
    cJSON *greeting_item = NULL;
    char *printed = NULL;
    double retrieved_number = 0.0;
    const char *retrieved_string = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddNullToObject(root, "nothing");
    arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddRawToObject(root, "payload", raw_json);

    // step 3: Operate / Validate
    arr_item = cJSON_GetArrayItem(arr, 1);
    retrieved_number = cJSON_GetNumberValue(arr_item);
    greeting_item = cJSON_GetObjectItem(root, "greeting");
    retrieved_string = cJSON_GetStringValue(greeting_item);
    printed = cJSON_PrintUnformatted(root);
    (void)retrieved_number;
    (void)retrieved_string;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}