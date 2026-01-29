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
//<ID> 1168
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
    cJSON *data = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(data, s1);
    cJSON_AddItemToArray(data, s2);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *scale = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(meta, "scale", scale);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "note", "initial");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(detached_meta);
    cJSON *scale_ref = cJSON_GetObjectItem(detached_meta, "scale");
    double old_scale = cJSON_GetNumberValue(scale_ref);
    double new_scale = cJSON_SetNumberHelper(scale_ref, old_scale * 2.0);
    cJSON_AddItemToObject(root, "meta", detached_meta);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_bool null_check = cJSON_IsNull(null_item);
    int data_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "data"));
    double computed_val = new_scale + (double)data_size;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    cJSON *note_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *note_str = cJSON_GetObjectItem(note_ref, "note");
    char *setret = cJSON_SetValuestring(note_str, "updated");
    (void)meta_is_obj;
    (void)null_check;
    (void)setret;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(null_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}