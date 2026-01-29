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
//<ID> 1836
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = "{\"arr\":[1, 2, 3], \"meta\":{\"name\":\"alpha\"}}";
    cJSON_Minify(json_buf);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);

    // step 2: Configure
    cJSON *added_meta = cJSON_AddObjectToObject(root, "added_meta");
    cJSON *orig_arr = cJSON_GetObjectItem(root, "arr");
    cJSON *dup_arr = cJSON_Duplicate(orig_arr, 1);
    cJSON_AddItemToObject(root, "dup_arr", dup_arr);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(dup_arr, 1);
    int orig_size = cJSON_GetArraySize(orig_arr);
    int dup_size = cJSON_GetArraySize(dup_arr);
    cJSON *first = cJSON_GetArrayItem(dup_arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *nameItem = cJSON_GetObjectItem(meta, "name");
    const char *name_str = cJSON_GetStringValue(nameItem);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (dup_size % 10));
    buffer[2] = (char)('0' + (orig_size % 10));
    buffer[3] = name_str ? name_str[0] : 'N';
    (void)first_val; (void)buffer; (void)parse_end; (void)added_meta;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}