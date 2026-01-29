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
//<ID> 1834
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_copy[] = "{\"arr\":[{\"id\":1},{\"id\":2}], \"meta\":{\"name\":\"alpha\"}}";
    cJSON_Minify(json_copy);
    cJSON *root = cJSON_Parse(json_copy);

    // step 2: Configure
    cJSON *added_meta = cJSON_AddObjectToObject(root, "added_meta");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_dup);

    // step 3: Operate and Validate
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    int original_size = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 0);
    int new_size = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON *id_item = cJSON_GetObjectItem(first_item, "id");
    double id_val = cJSON_GetNumberValue(id_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "name"));
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (((int)id_val) % 10));
    buffer[2] = name_str[0];
    (void)original_size; (void)new_size; (void)added_meta; (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}