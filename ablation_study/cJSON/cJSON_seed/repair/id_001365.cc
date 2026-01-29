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
//<ID> 1365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num_item = cJSON_CreateNumber(137.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON_AddItemToArray(arr, str_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObject(root, "raw_section", raw_item);

    // step 2: Configure
    cJSON *info = cJSON_CreateObject();
    cJSON_AddStringToObject(info, "purpose", "api_sequence_test");
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddNumberToObject(root, "priority", 5.0);

    // step 3: Operate and Validate
    double first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    char *second_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(96);
    memset(scratch, 0, 96);
    char *buf = (char *)scratch;
    buf[0] = out ? out[0] : 0;
    buf[1] = second_str ? second_str[0] : 0;
    buf[2] = (char)('0' + ((int)first_num % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}