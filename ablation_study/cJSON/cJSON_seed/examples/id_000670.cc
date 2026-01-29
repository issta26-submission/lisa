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
//<ID> 670
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *base_num = cJSON_CreateNumber(4.5);
    cJSON *dup_num = cJSON_Duplicate(base_num, 1);

    // step 2: Configure
    cJSON_AddItemToArray(arr, dup_num);
    cJSON_AddItemReferenceToArray(arr, base_num);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddNumberToObject(root, "count", 2.0);
    double updated = cJSON_SetNumberHelper(base_num, 9.75);
    (void)version;
    (void)updated;

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "array");
    int parsed_size = cJSON_GetArraySize(parsed_arr);
    cJSON *first_item = cJSON_GetArrayItem(parsed_arr, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    (void)parsed_size;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}