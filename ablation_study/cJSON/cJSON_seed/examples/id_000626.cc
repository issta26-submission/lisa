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
//<ID> 626
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    int numbers[] = {10, 20, 30, 40};
    cJSON *ints_array = cJSON_CreateIntArray(numbers, 4);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "ints", ints_array);
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *optional_null = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    int array_size = cJSON_GetArraySize(ints_array);
    char *status_buf = (char *)cJSON_malloc(256);
    memset(status_buf, 0, 256);
    sprintf(status_buf, "ver=%s;count=%d", version ? version : "null", array_size);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_ints = cJSON_GetObjectItem(parsed, "ints");
    cJSON *first_item = cJSON_GetArrayItem(parsed_ints, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON_bool optional_was_null = cJSON_IsNull(cJSON_GetObjectItem(parsed, "optional"));
    (void)first_value;
    (void)optional_was_null;

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(parsed, "optional");
    cJSON_free(snapshot);
    cJSON_free(status_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}