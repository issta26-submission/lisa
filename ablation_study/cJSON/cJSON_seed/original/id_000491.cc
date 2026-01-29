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
//<ID> 491
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[3.5,4.5],\"flag\":true}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);

    // step 2: Configure
    cJSON_AddNullToObject(root, "note");
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    double computed = first_val + (double)arr_is_array;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 128;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    char *dup_snapshot = cJSON_PrintUnformatted(duplicate);

    // step 4: Validate & Cleanup
    double dup_first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(duplicate, "arr"), 0));
    (void)dup_first_val;
    cJSON_free(snapshot);
    cJSON_free(dup_snapshot);
    cJSON_free(buffer);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}