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
//<ID> 778
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"array\":[10,20,30],\"name\":\"alpha\"}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, sizeof(json_text) - 1, &parse_end, 1);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddRawToObject(root, "rawdata", "{\"inner\":123}");
    cJSON_AddNullToObject(root, "maybe");
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(100);
    cJSON *n2 = cJSON_CreateNumber(200);
    cJSON *n3 = cJSON_CreateNumber(300);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *fetched_name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    char *name_val = cJSON_GetStringValue(fetched_name);
    cJSON_AddStringToObject(root, "name_copy", name_val);

    // step 3: Operate
    cJSON *numbers_array = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    cJSON *detached = cJSON_DetachItemFromArray(numbers_array, 1);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}