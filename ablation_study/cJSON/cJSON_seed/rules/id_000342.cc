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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"items\":[\"one\",\"two\"]}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *array = cJSON_GetObjectItem(root, "items");
    cJSON *new_str = cJSON_CreateString("three");
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool is_str = 0;
    cJSON_bool cmp = 0;
    cJSON_bool prealloc_ok = 0;

    // step 2: Setup / Configure
    cJSON_InsertItemInArray(array, 1, new_str);
    cJSON_AddItemReferenceToArray(array, first_item);

    // step 3: Operate / Validate
    cJSON *inserted = cJSON_GetArrayItem(array, 1);
    is_str = cJSON_IsString(inserted);
    cJSON *ref_appended = cJSON_GetArrayItem(array, 3);
    cmp = cJSON_Compare(first_item, ref_appended, 1);
    printed = cJSON_PrintUnformatted(root);
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    buffer[0] = (char)('0' + (int)is_str);
    buffer[1] = (char)('0' + (int)cmp);
    (void)prealloc_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}