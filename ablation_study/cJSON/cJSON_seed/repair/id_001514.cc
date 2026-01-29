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
//<ID> 1514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "initial");
    cJSON *version_item = cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *enabled_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled_flag);
    cJSON *list = cJSON_AddArrayToObject(root, "items");
    cJSON *elem1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(list, elem1);
    cJSON *elem2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(list, elem2);
    cJSON *note = cJSON_CreateString("note");
    cJSON_AddItemToObject(meta, "note", note);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name_item, "updated_name");
    char *created_str = cJSON_CreateString("created_child")->valuestring; /* use created string value for validation */

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    const char *name_val = cJSON_GetStringValue(name_item);
    double ver_val = cJSON_GetNumberValue(version_item);
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_flag);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = name_val ? name_val[0] : 'x';
    scratch[1] = (char)('0' + ((int)ver_val % 10));
    scratch[2] = (char)('0' + (int)is_enabled);
    scratch[3] = out_len ? out[0] : ' ';
    scratch[4] = set_ret ? set_ret[0] : 'y';
    scratch[5] = created_str ? created_str[0] : 'z';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}