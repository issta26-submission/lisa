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
//<ID> 1493
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
    cJSON_bool rc_meta = cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("tester");
    cJSON_bool rc_name = cJSON_AddItemToObject(meta, "name", name);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON *note = cJSON_CreateString("initial");
    cJSON_bool rc_note = cJSON_AddItemToObject(root, "note", note);
    char *set_res = cJSON_SetValuestring(note, "updated");
    (void)set_res;
    cJSON *child = cJSON_CreateObject();
    cJSON_bool rc_child = cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate and Validate
    char *name_str = cJSON_GetStringValue(name);
    char *note_str = cJSON_GetStringValue(note);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = note_str ? note_str[0] : 'y';
    scratch[2] = (char)('0' + (int)cJSON_IsTrue(active_item));
    scratch[3] = out_len ? out[0] : ' ';
    scratch[4] = (char)('0' + (int)rc_meta);
    scratch[5] = (char)('0' + (int)rc_name);
    scratch[6] = (char)('0' + (int)rc_note);
    scratch[7] = (char)('0' + (int)rc_child);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}