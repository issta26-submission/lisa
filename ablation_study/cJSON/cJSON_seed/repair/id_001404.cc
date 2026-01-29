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
//<ID> 1404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddNumberToObject(meta, "id", 7.0);
    cJSON_AddTrueToObject(meta, "active");
    cJSON *placeholder = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, placeholder);

    // step 2: Configure
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_ReplaceItemInArray(items, 0, dup_meta);

    // step 3: Operate and Validate
    cJSON *id_item = cJSON_GetObjectItem(meta, "id");
    double id_val = cJSON_GetNumberValue(id_item);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *first_id = cJSON_GetObjectItem(first_item, "id");
    double first_id_val = cJSON_GetNumberValue(first_id);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *s = (char *)scratch;
    s[0] = printed ? 'P' : 'p';
    s[1] = (char)('0' + ((int)id_val % 10));
    s[2] = (char)('0' + ((int)first_id_val % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}